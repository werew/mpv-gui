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
    qDebug() << "new connection";
    QLocalSocket *newclient = myserver->nextPendingConnection();
    newclient->write("Welcome to mpv-gui server!\n");
    clients->append(newclient);
    connect(newclient, SIGNAL(disconnected()),this,SLOT(removeClient()));
    connect(newclient, SIGNAL(readyRead()),this,SLOT(readFromClient()));
}

void Server::readFromClient(){
    // TODO read until buffer is empty
}

void Server::removeClient(){
   // TODO remove client from the list
}
