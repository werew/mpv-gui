//Author : CONGILIO Luigi    CONSTANS Victor

#include "server.h"


Server::Server(QObject *parent, char* configfile) :
    QObject(parent),
    myserver(new QLocalServer(this)),
    clients(new QList<QGuiClientSocket*>()),
    mpv(new QMpvSocket())
{
    /* Set default values */
    type_stream = NONE;
    metadata = QJsonObject();
    pause = false;
    stop = true;
    stream = "";
    percent_pos = 0;
    volume = 100;

    importConfig(configfile);
    connect(myserver, SIGNAL(newConnection()),this, SLOT(handleConnection()));
    connect(mpv, SIGNAL(readyRead()),this,SLOT(readFromMpv()));

    /* Run mpv if not running ? */
    mpv->connectToServer("/tmp/mpvsocket"); 
    if (mpv->waitForConnected() == false)
        throw std::runtime_error("Failed to connect to mpv\n"+
            mpv->errorString().toStdString());

    bindProperties();
}


/**
 * @desc Create socket and start to listen
 *       for incoming connections
 */
void Server::listen(void){
    QLocalServer::removeServer(SERVER_NAME);
    if (!myserver->listen(SERVER_NAME)) {
        qDebug() << "Error: cannot launch server";
        return;
    }
    qDebug() << "Server up and listening";
}


/**
 * @desc Accept new connection from a client
 */
void Server::handleConnection(void){
    qDebug("~> New connection");

    // Add client to the list of open connections
    QGuiClientSocket *newclient = new QGuiClientSocket(this, myserver->nextPendingConnection());
    clients->append(newclient);

    // Be ready to read or close the connection when it's time
    connect(newclient, SIGNAL(readyRead()),this,SLOT(readFromClient()));
    connect(newclient, SIGNAL(disconnected()),this,SLOT(removeClient()));

    // Send the complete state
    sendAll(newclient);
}


/**
 * @desc Send all the status to the client
 * @param client Socket to the client
 */
void Server::sendAll(QGuiClientSocket *client){
    client->config(&config);

    if (stop) client->stop();
    else {
        if (pause) client->pause();
        else client->unpause();
    }

    client->volume(volume);
    client->meta(&metadata);
    client->percent_pos(percent_pos);
    client->time_pos(time_pos);
    client->duration(duration);
}




/**
 * @desc Receive a message from mpv
 */
void Server::readFromMpv(){
    while (true) {
        QByteArray a = mpv->readLine(MAX_SIZECMD);
        if (a.isEmpty()) break;

        QJsonParseError error;
        QJsonDocument jDoc = QJsonDocument::fromJson(a, &error);
        if (jDoc.isNull()){
            qDebug() << "Parsing error: "+ error.errorString();
            continue;
        }
        QJsonObject jsonObject = jDoc.object();
        handleMpvMsg(jsonObject);
    }
}

/**
 * @desc Manage mpv's updates
 * @param o JsonObject sent by mpv
 */
void Server::handleMpvMsg(QJsonObject o){
   if (o.contains("event") == false) return;

   QString event = o["event"].toString() ;
   if (event != "property-change") return;

   QJsonObject m;

   switch (o["id"].toInt()){
       // volume
       case 1: volume = o["data"].toInt();
               for (int i = 0; i < clients->count(); i++)
                   clients->at(i)->volume(volume);
             break;
       // percent-pos 
       case 2: percent_pos = o["data"].toDouble();
               for (int i = 0; i < clients->count(); i++)
                   clients->at(i)->percent_pos(percent_pos);
             break;
       // path
       case 3: loadFile_res(o["data"].toString());
             break;
       // pause
       case 4: pause = o["data"].toBool();
               for (int i = 0; i < clients->count(); i++)
                   if (pause) clients->at(i)->pause();
                   else if (stop == false)
                       clients->at(i)->unpause();
             break;
       // idle
       case 5: stop = o["data"].toBool();
               for (int i = 0; i < clients->count(); i++)
                   if (stop) clients->at(i)->stop();
                   else if (pause) clients->at(i)->pause();
                   else clients->at(i)->unpause();
             break;
       // meta
       case 6: m = o["data"].toObject();
               // Use mpv's metadatas only for radios
               if (m.contains("icy-name")){
                   metadata = m;
                   for (int i = 0; i < clients->count(); i++)
                       clients->at(i)->meta(&metadata);
               }
             break;
       // time-pos
       case 7: time_pos = o["data"].toDouble();
               for (int i = 0; i < clients->count(); i++)
                   clients->at(i)->time_pos(time_pos);
             break;
       // duration
       case 8: duration = o["data"].toDouble();
               for (int i = 0; i < clients->count(); i++)
                   clients->at(i)->duration(duration);
             break;
   }
}


/**
 * @desc Receive a message from a client
 */
void Server::readFromClient(){
    QGuiClientSocket* socket = qobject_cast<QGuiClientSocket*>(sender());

    // Read until buffer is empty
    while (true) {
        QByteArray a = socket->readLine(MAX_SIZECMD);
        if (a.isEmpty()) break;

        QJsonParseError error;
        QJsonDocument jDoc = QJsonDocument::fromJson(a, &error);
        if (jDoc.isNull()){
            qDebug() << "Parsing error: "+ error.errorString();
            qDebug() << a;
            continue;
        }
        QJsonObject jsonObject = jDoc.object();
        handleClientMsg(jsonObject);
    }
}


/**
 * @desc Manage client message
 * @param o JsonObject sent by the client
 */
void Server::handleClientMsg(QJsonObject o){

   QJsonObject params;

   switch (o["type"].toInt()){
       case STOP: mpv->stop();
             break;
       case PAUSE: mpv->pause(true);
             break;
       case UNPAUSE: mpv->pause(false);
             break;
       case PERCENT_POS: mpv->percent_pos(o["data"].toDouble());
             break;
       case VOLUME: mpv->volume(o["data"].toInt());
             break;
       case LOADPIECE: loadPiece(o["data"].toString());
             break;
       case LOADRADIO: loadRadio(o["data"].toString());
             break;
       case LOADLIST: params = o["data"].toObject();
                      loadList(params["list"].toString(),
                               params["start"].toInt());
             break;
       case STEP_FOR: mpv->step_forward();
             break;
       case STEP_BACK: mpv->step_backward();
             break;
       case NEXT: loadNext();
             break;
       case PREV: loadPrevious();
             break;
    }

}


/**
 * @desc Move to next stream on the config
 */
void Server::loadNext(){
    QJsonObject o;
    int i;

    switch (type_stream){
       // Use mpv command for playlists
       case PLAYLIST: mpv->pl_next();
                      return;
            break;
       case RADIO: if (!config.contains("Radios")) return;
                   o = config["Radios"].toObject();
            break;
       case PIECE: if (!config.contains("Pieces")) return;
                   o = config["Pieces"].toObject();
            break;
    }

    QStringList k = o.keys();
    i = k.indexOf(stream) + 1;
    if (i < k.size()){
        stream = k.at(i);
        QString path = o[stream].toString();
        mpv->load_file(path);
        loadings.insert(path, stream);
    }
}


/**
 * @desc Move to previous stream on the config
 */
void Server::loadPrevious(){
    QJsonObject o;
    int i;

    switch (type_stream){
       // Use mpv command for playlists
       case PLAYLIST: mpv->pl_prev();
                      return;
            break;
       case RADIO: if (!config.contains("Radios")) return;
                   o = config["Radios"].toObject();
            break;
       case PIECE: if (!config.contains("Pieces")) return;
                   o = config["Pieces"].toObject();
            break;
    }

    QStringList k = o.keys();
    i = k.indexOf(stream) - 1;
    if (i >= 0){
        stream = k.at(i);
        QString path = o[stream].toString();
        mpv->load_file(path);
        loadings.insert(path, stream);
    }
}



/**
 * @desc Read and import config file
 * @param filename Path to the config file
 */
void Server::importConfig(const char* filename){
        QFile file(filename);
        if(!file.open(QIODevice::ReadOnly)) {
            throw std::runtime_error(file.errorString().toStdString());
        }

        QTextStream in(&file);
        QString text = in.readAll();
        file.close();

        QJsonParseError error;
        QJsonDocument jDoc = QJsonDocument::fromJson(text.toUtf8(), &error);
        if (jDoc.isNull()){
            throw std::runtime_error("bad config file ("+
                error.errorString().toStdString()+")");
        }
        config = jDoc.object();
}


/**
 * @desc Remove disconnected client
 */
void Server::removeClient(){
    // Remove socket from the list of active connections
    QGuiClientSocket* socket = qobject_cast<QGuiClientSocket*>(sender());
    clients->removeOne(socket);
    delete socket;
    qDebug("~> A client left");
}


/**
 * @desc Load a playlist into mpv
 * @param list Name of the playlist
 * @param index Index of the entry point
 */
void Server::loadList(QString list, int index){
      QJsonObject pl = config["Playlists"].toObject();
      QJsonObject items = pl[list].toObject();
      QStringList titles = items.keys();
      mpv->stop();
      for (int i = 0; i < titles.size(); i++){
          QString path  = items[titles.at(i)].toString();
          mpv->append(path);
          loadings.insert(path, items.keys().at(i));
      }
      mpv->set_property("playlist-pos",index);
      stream = list;
      type_stream = PLAYLIST;
}


/**
 * @desc Observe mpv's properties
 */
void Server::bindProperties(){
    mpv->observe_property(1, "volume");
    mpv->observe_property(2, "percent-pos");
    mpv->observe_property(3, "path");
    mpv->observe_property(4,"pause");
    mpv->observe_property(5,"idle-active");
    mpv->observe_property(6,"metadata");
    mpv->observe_property(7,"time-pos");
    mpv->observe_property(8,"duration");
}


/**
 * @desc Get metadata from file
 * @param fileName path to the file
 */
QJsonObject Server::getTags(QString fileName) {
    QJsonObject json_tags = QJsonObject();
    TagLib::FileRef f(fileName.toLatin1().data());

    if(!f.isNull() && f.tag()) {

        TagLib::PropertyMap tags = f.file()->properties();

        for(TagLib::PropertyMap::ConstIterator i=tags.begin();
            i != tags.end(); ++i) {
            QString key = QString::fromStdString(i->first.to8Bit(true));
            QString value = QString();

            for(TagLib::StringList::ConstIterator j=i->second.begin();
                j != i->second.end(); ++j) {
                value += QString::fromStdString(j->to8Bit(true));
            }

            json_tags.insert(key,value);
        }
    }
    return json_tags;
}


/**
 * @desc Load piece into mpv
 * @param name Id of the file (config)
 */
void Server::loadPiece(QString name){
   QJsonObject o;
   QString path;
   o = config["Pieces"].toObject();
   if (o.contains(name) && o[name].isString()){
       path = o[name].toString();
       mpv->load_file(path);
       stream = name;
       if (type_stream == PLAYLIST)
            loadings.clear();
       type_stream = PIECE;
       loadings.insert(path, name);
   }
}


/**
 * @desc Load Radio station
 * @param name Id the radio (config)
 */
void Server::loadRadio(QString name){
   QJsonObject o;
   QString path;
   o = config["Radios"].toObject();
   if (o.contains(name) && o[name].isString()){
       path = o[name].toString();
       mpv->load_file(path);
       stream = name;
       if (type_stream == PLAYLIST)
            loadings.clear();
       type_stream = RADIO;
       loadings.insert(path, name);
   }
}


/**
 * @desc Handle load-file mpv's response
 * @param path Path to the loaded file
 */
void Server::loadFile_res(QString path){
       metadata = getTags(path);

       QString name = loadings.contains(path) ?
                      loadings[path] : "";

       if (metadata.contains("TITLE")){
           metadata.insert("TITLE",metadata["TITLE"]
                   .toString().trimmed());
       }

       if (!metadata.contains("TITLE") ||
            metadata["TITLE"].toString().isEmpty()){
           metadata.insert("TITLE",name);
       }

       if (type_stream != PLAYLIST)
           loadings.remove(path);

       for (int i = 0;  i < clients->count(); i++){
           QGuiClientSocket *c = clients->at(i);
           c->load(name);
           c->meta(&metadata);
       }
   }
