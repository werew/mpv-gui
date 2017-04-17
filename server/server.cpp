#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent),
    myserver(new QLocalServer(this))
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
}
