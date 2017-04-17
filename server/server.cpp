#include "server.h"

Server::Server(QObject *parent) :
    QObject(parent),
    myserver(new QLocalServer(this))
{
    running = true;
    loopThread = QtConcurrent::run(this, &Server::clientLoop);
}

void Server::clientLoop(void){
    qDebug() << "hello";

}
