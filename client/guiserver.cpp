//Author : CONGILIO Luigi    CONSTANS Victor

#include "guiserver.h"

GuiServer::GuiServer(QObject *parent) :
     QLocalSocket(parent)
{
}

GuiServer::~GuiServer(){
    //delete socket;
}

QString GuiServer::jsonToString(QJsonObject *o){
    QJsonDocument dc = QJsonDocument(*o);
    return dc.toJson(QJsonDocument::Compact);
}

void GuiServer::pause(){
   this->send_command(PAUSE);
}


void GuiServer::loadPiece(const QString file){
   this->send_command(PIECELOAD, "\""+file+"\"");
}

void GuiServer::loadRadio(const QString file){
   this->send_command(RADIOLOAD, "\""+file+"\"");
}

void GuiServer::loadPlaylist(const QString playlist, int nb){
   this->send_command(LISTLOAD, "{\"list\": \""+playlist+"\","+
                                " \"start\":"+QString::number(nb)+
                                "}");

}

void GuiServer::percent_pos(double pos){
   qDebug() << "Position: " << pos << endl;
   this->send_command(PERCENT_POS, QString::number(pos));
}

void GuiServer::unpause(){
   this->send_command(UNPAUSE);
}

void GuiServer::volume(int value){
    this->send_command(VOLUME, QString::number(value));
}

void GuiServer::stop(){
    this->send_command(STOP);
}

void GuiServer::step_forward(){
    this->send_command(STEP_FOR);
}

void GuiServer::step_backward(){
    this->send_command(STEP_BACK);
}

void GuiServer::next(){
    this->send_command(NEXT);
}

void GuiServer::prev(){
    this->send_command(PREV);
}


void GuiServer::send_command(int type){
    QString json_cmd = QString("{\"type\":"+QString::number(type)+"}\n");
    this->write(json_cmd.toUtf8());
    this->flush();
}


void GuiServer::send_command(int type, const QString data){
    QString json_cmd = QString("{\"type\":"+QString::number(type)+
                               ",\"data\":"+data+"}\n");
    this->write(json_cmd.toUtf8());
    this->flush();
}
