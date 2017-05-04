//Author : CONGILIO Luigi    CONSTANS Victor

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

QString QGuiClientSocket::jsonToString(QJsonObject *o){
    QJsonDocument dc = QJsonDocument(*o);
    return dc.toJson(QJsonDocument::Compact);
}

void QGuiClientSocket::pause(){
    qDebug() << "Sending pause " << PAUSE ;
   this->send_command(PAUSE);
}

void QGuiClientSocket::meta(const QString meta){
   this->send_command(META, meta);
}
void QGuiClientSocket::meta(QJsonObject *meta){
   this->meta(jsonToString(meta));
}

void QGuiClientSocket::load(const QString file){
   this->send_command(LOAD, "\""+file+"\"");
}

void QGuiClientSocket::percent_pos(double pos){
   this->send_command(PERCENT_POS, QString::number(pos));
}
void QGuiClientSocket::time_pos(double pos){
   this->send_command(TIME_POS, QString::number(pos));
}

void QGuiClientSocket::duration(double duration){
   this->send_command(DURATION, QString::number(duration));
}

void QGuiClientSocket::unpause(){
   qDebug() << "Sending unpause" << PAUSE ;
   this->send_command(UNPAUSE);
}

void QGuiClientSocket::volume(int value){
    this->send_command(VOLUME, QString::number(value));
}

void QGuiClientSocket::stop(){
    this->send_command(STOP);
}

void QGuiClientSocket::config(const QString conf){
    this->send_command(CONFIG, conf);
}

void QGuiClientSocket::config(QJsonObject *conf){
    this->config(jsonToString(conf));
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
