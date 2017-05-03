#include "server.h"


Server::Server(QObject *parent, char* configfile) :
    QObject(parent),
    myserver(new QLocalServer(this)),
    clients(new QList<QGuiClientSocket*>()),
    mpv(new QMpvSocket())
{
    /* Set default values */
    metadata = QJsonObject();
    pause = false;
    stop = true;
    stream = "";
    percent_pos = 0;
    volume = 100;

    importConfig(configfile);
    connect(myserver, SIGNAL(newConnection()),this, SLOT(handleConnection()));
    connect(mpv, SIGNAL(readyRead()),this,SLOT(readFromMpv()));

    // Run mpv if not running ?
    mpv->connectToServer("/tmp/mpvsocket"); // TODO get this path from the cli
    if (mpv->waitForConnected() == false)
        throw std::runtime_error("Failed to connect to mpv\n"+
                                 mpv->errorString().toStdString());
    bindProperties();
}


void Server::listen(void){
    QLocalServer::removeServer(SERVER_NAME);
    if (!myserver->listen(SERVER_NAME)) {
        qDebug() << "Error: cannot launch server";
        return;
    }
    qDebug() << "Server up and listening";
}

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

void Server::handleMpvMsg(QJsonObject o){
   if (o.contains("event") == false) return;

   QString event = o["event"].toString() ;
   if (event != "property-change") return;

   QJsonObject m;

   switch (o["id"].toInt()){
       case 1: volume = o["data"].toInt();
               for (int i = 0; i < clients->count(); i++)
                   clients->at(i)->volume(volume);
             break;
       case 2: percent_pos = o["data"].toDouble();
               for (int i = 0; i < clients->count(); i++)
                   clients->at(i)->percent_pos(percent_pos);
             break;
       case 3: loadFile_res(o["data"].toString());
             break;
       case 4: pause = o["data"].toBool();
               for (int i = 0; i < clients->count(); i++)
                   if (pause) clients->at(i)->pause();
                   else clients->at(i)->unpause();
             break;
       case 5: stop = o["data"].toBool();
               for (int i = 0; i < clients->count(); i++)
                   if (stop) clients->at(i)->stop();
                   else if (pause) clients->at(i)->pause();
                   else clients->at(i)->unpause();
             break;
       case 6: m = o["data"].toObject();
               // Use mpv's metadatas only for radios
               if (m.contains("icy-name")){
                   metadata = m;
                   for (int i = 0; i < clients->count(); i++)
                       clients->at(i)->meta(&metadata);
               }
             break;
       case 7: time_pos = o["data"].toDouble();
               for (int i = 0; i < clients->count(); i++)
                   clients->at(i)->time_pos(time_pos);
             break;
       case 8: duration = o["data"].toDouble();
               for (int i = 0; i < clients->count(); i++)
                   clients->at(i)->duration(duration);
             break;
   }
  // qDebug() << "pause:" << pause << " vol:" << volume << " pos:" << percent_pos
  //          << "file:" << stream;
}

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
       case LOAD: mpv->load_file(o["data"].toString());
             break;
       case LOADLIST: params = o["data"].toObject();
                      loadList(params["list"].toString(),
                               params["start"].toInt());
             break;
       case STEP_FOR: mpv->step_forward();
             break;
       case STEP_BACK: mpv->step_backward();
             break;
       case PL_NEXT: mpv->pl_next();
             break;
       case PL_PREV: mpv->pl_prev();
             break;
    }

}


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


void Server::removeClient(){
    // Remove socket from the list of active connections
    QGuiClientSocket* socket = qobject_cast<QGuiClientSocket*>(sender());
    clients->removeOne(socket);
    delete socket;
    qDebug("~> A client left");
}

void Server::loadList(QString list, int index){
      qDebug() << list;
      qDebug() << "Charge";
      QJsonObject pl = config["Playlists"].toObject();
      qDebug() << pl.keys();
      QJsonObject items = pl[list].toObject();
      QStringList titles = items.keys();
      mpv->stop();
      for (int i = 0; i < titles.size(); i++){
          qDebug() << "Garga";
          qDebug() << titles.at(i);
          mpv->append(items[titles.at(i)].toString());
      }
      mpv->set_property("playlist-pos",index);
      qDebug() << "______________________> " << index;
}

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

/*
QMap<QString, QString> Server::getTags(QString fileName) {
    QMap <QString, QString> tagMap;
    TagLib::FileRef f(fileName.toLatin1().data());
    if(!f.isNull() && f.tag()) {
        TagLib::PropertyMap tags = f.file()->properties();
        for(TagLib::PropertyMap::ConstIterator i=tags.begin();
            i != tags.end(); ++i) {
            QString key = QString::fromStdString(i->first.to8Bit(true));
            tagMap[key] = QString();
            for(TagLib::StringList::ConstIterator j=i->second.begin();
                j != i->second.end(); ++j) {
                tagMap[key] += QString::fromStdString(j->to8Bit(true));
            }
           qDebug() << key << ": " << tagMap[key];
        }
    }
    return tagMap;
} */

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

void Server::loadFile_req(QString filename){
   if (config.contains(filename) == false) return;
   QString path = config.value(filename).toString();
   loadings.insert(path, filename);
   mpv->load_file(path);
}

void Server::loadFile_res(QString path){
       stream = path;
       metadata = getTags(stream);
       QString name = loadings.contains(stream) ? loadings[stream] : "null";
       loadings.remove(stream);
       for (int i = 0;  i < clients->count(); i++){
           QGuiClientSocket *c = clients->at(i);
           c->load(name);
           c->meta(&metadata);
       }
   }
