#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent),
    myserver(new QLocalServer(this)),
    clients(new QList<QLocalSocket*>())
{
    running = true;
    loopThread = QtConcurrent::run(this, &Server::clientLoop);
    connect(myserver, SIGNAL(newConnection()),this, SLOT(handleConnection()));
}

void Server::clientLoop(void){
    qDebug() << "hello";

}

void Server::listen(void){
    QLocalServer::removeServer(SERVER_NAME);
    if (!myserver->listen(SERVER_NAME)) {
        qDebug() << "problem";
        return;
    }
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
    // TODO read until buffer is empty
}

void Server::removeClient(){
    // Remove socket from the list of active connections
    QLocalSocket* socket = qobject_cast<QLocalSocket*>(sender());
    clients->removeOne(socket);
    delete socket;
    qDebug("~> A client left");
}
