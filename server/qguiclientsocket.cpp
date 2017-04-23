#include "qguiclientsocket.h"

QGuiClientSocket::QGuiClientSocket(QObject *parent, QLocalSocket *socket) :
    QObject(parent)
{
    this->socket = socket;
    if (socket->open() == false)
        throw std::runtime_error("Failed to open socket\n");
    connect(this->socket, SIGNAL(readyRead()),this,SLOT(signal_readyRead()));
    connect(this->socket, SIGNAL(disconnected()),this,SLOT(signal_disconnected()));
}

QGuiClientSocket::~QGuiClientSocket(){
    delete socket;
}

void QGuiClientSocket::pause(){
   this->send_command(PAUSE);
}

void QGuiClientSocket::unpause(){
   this->send_command(UNPAUSE);
}

void QGuiClientSocket::volume(int value){
    this->send_command(VOLUME, QString::number(value));
}

void QGuiClientSocket::stop(){
    this->send_command(STOP);
}

void QGuiClientSocket::config(const QString data){
    this->send_command(CONFIG, data);
}

void QGuiClientSocket::send_command(int type){
    QString json_cmd = QString("{\"type\":"+QString::number(type)+"}\n");
    socket->write(json_cmd.toUtf8());
    socket->flush();
}

void QGuiClientSocket::send_command(int type, const QString data){
    QString json_cmd = QString("{\"type\":"+QString::number(type)+
                               ",\"data\":"+data+"}\n");
    socket->write(json_cmd.toUtf8());
    socket->flush();
}

QByteArray QGuiClientSocket::readLine(qint64 maxSize){
   return this->socket->readLine(maxSize);
}

void QGuiClientSocket::signal_readyRead(){ emit readyRead(); }
void QGuiClientSocket::signal_disconnected(){ emit disconnected(); }
