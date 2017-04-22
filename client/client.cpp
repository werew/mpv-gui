#include "client.h"

Client::Client(QObject *parent,char* configfile) :
    QObject(parent),
    myserver(new QLocalServer(this)),
    clients(new QList<QLocalSocket*>()),
    server(new GuiServer())
{
    /* Set default values */
    pause = false;
    stop = true;
    filename = "";
    percent_pos = 0;
    volume = 100;

    importConfig(configfile);
    connect(myserver, SIGNAL(newConnection()),this, SLOT(handleConnection()));
    connect(server, SIGNAL(readyRead()),this,SLOT(readFromMpv()));

    // Run mpv if not running ?
    server->connectToServer("/tmp/mpvsocket"); // TODO get this path from the cli
    if (server->waitForConnected() == false)
        throw std::runtime_error("Failed to connect to mpv\n"+
                                 server->errorString().toStdString());
    bindProperties();
}


void Client::listen(void){
    QLocalServer::removeServer(SERVER_NAME);
    if (!myserver->listen(SERVER_NAME)) {
        qDebug() << "Error: cannot launch server";
        return;
    }
    qDebug() << "Server up and listening";
}

void Client::handleConnection(void){
    qDebug("~> New connection");

    // Add client to the list of open connections
    QLocalSocket *newclient = myserver->nextPendingConnection();
    clients->append(newclient);

    // Be ready to read or close the connection when it's time
    connect(newclient, SIGNAL(readyRead()),this,SLOT(readFromClient()));
    connect(newclient, SIGNAL(disconnected()),this,SLOT(removeClient()));

    // Always be polite
    newclient->write("Welcome to mpv-gui server!\n");
}

void Client::readFromClient(){
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());

    // Read until buffer is empty
    while (true) {
        QByteArray a = socket->readLine(MAX_SIZECMD);
        if (a.isEmpty()) break;

        QJsonParseError error;
        QJsonDocument jDoc = QJsonDocument::fromJson(a, &error);
        if (jDoc.isNull()){
            qDebug() << "Parsing error: "+ error.errorString();
            continue;
        }
        QJsonObject jsonObject = jDoc.object();
        // TODO handle request
    }
}

void Client::handleMpvMsg(QJsonObject o){
   if (o.contains("event") == false) return;

   QString event = o["event"].toString() ;
   if (event != "property-change") return;

   switch (o["id"].toInt()){
       case 1: volume = o["data"].toInt();
         break;
       case 2: percent_pos = o["data"].toDouble();
             break;
       case 3: filename = o["data"].toString();
             break;
       case 4: pause = o["data"].toBool();
             break;
       case 5: stop = o["data"].toBool();
             break;
   }
   qDebug() << "pause:" << pause << " vol:" << volume << " pos:" << percent_pos
            << "file:" << filename;
}

void Client::handleClientMsg(QJsonObject o){

}

void Client::readFromMpv(){
    while (true) {
        QByteArray a = server->readLine(MAX_SIZECMD);
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


void Client::importConfig(const char* filename){
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


void Client::removeClient(){
    // Remove socket from the list of active connections
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    clients->removeOne(socket);
    delete socket;
    qDebug("~> A client left");
}

void Client::bindProperties(){
    server->observe_property(1, "volume");
    server->observe_property(2, "percent-pos");
    server->observe_property(3, "filename");
    server->observe_property(4,"pause");
    server->observe_property(5,"idle-active");
}
