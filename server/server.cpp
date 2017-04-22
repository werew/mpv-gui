#include "server.h"

Server::Server(QObject *parent, char* configfile) :
    QObject(parent),
    myserver(new QLocalServer(this)),
    clients(new QList<QLocalSocket*>()),
    mpv(new QMpvSocket())
{
    importConfig(configfile);
    connect(myserver, SIGNAL(newConnection()),this, SLOT(handleConnection()));
    connect(mpv, SIGNAL(readyRead()),this,SLOT(readFromMpv()));

    // Run mpv if not running ?
    mpv->connectToServer("/tmp/mpvsocket"); // TODO get this path from the cli
    if (mpv->waitForConnected() == false)
        throw std::runtime_error("Failed to connect to mpv\n"+
                                 mpv->errorString().toStdString());
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
    QLocalSocket *newclient = myserver->nextPendingConnection();
    clients->append(newclient);

    // Be ready to read or close the connection when it's time
    connect(newclient, SIGNAL(readyRead()),this,SLOT(readFromClient()));
    connect(newclient, SIGNAL(disconnected()),this,SLOT(removeClient()));

    // Always be polite
    newclient->write("Welcome to mpv-gui server!\n");
}

void Server::readFromClient(){
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
        qDebug() << a;
        // TODO handle request
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
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    clients->removeOne(socket);
    delete socket;
    qDebug("~> A client left");
}
