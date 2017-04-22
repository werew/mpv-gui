#include "guiserver.h"

GuiServer::GuiServer(QObject *parent) :
     QLocalSocket(parent)
{
}

QString GuiServer::quoted(const QString &s){
    return "\""+s+"\"";
}


void GuiServer::observe_property(int id, const QString &prop){
    const QString cmd[] = {"\"observe_property\"",
                           QString::number(id),
                           quoted(prop), NULL};
    this->send_command(cmd);
}

void GuiServer::set_property(const QString &prop, int value){
    const QString cmd[] = {"\"set_property\"", quoted(prop),
                           QString::number(value), NULL};
    this->send_command(cmd);
}

void GuiServer::set_property(const QString &prop, const QString &value){
    const QString cmd[] = {"\"set_property\"", quoted(prop),
                           quoted(value), NULL};
    this->send_command(cmd);
}

void GuiServer::set_property(const QString &prop, bool value){
    QString _value = value ? "true" : "false";
    const QString cmd[] = {"\"set_property\"", quoted(prop),
                           quoted(_value), NULL};
    this->send_command(cmd);
}

void GuiServer::step_backward(){
    const QString cmd[] = {"\"seek\"","-0.2","\"relative\"", NULL};
    this->send_command(cmd);
}

void GuiServer::step_forward(){
    const QString cmd[] = {"\"seek\"","0.2","\"relative\"", NULL};
    this->send_command(cmd);
}

void GuiServer::pause(bool value){
    this->set_property("pause",value);
}

void GuiServer::volume(int value){
    this->set_property("volume",value);
}

void GuiServer::stop(){
    const QString cmd[] = {"\"stop\"",NULL};
    this->send_command(cmd);
}

void GuiServer::pl_next(){
    const QString cmd[] = {"\"playlist-next\"",NULL};
    this->send_command(cmd);
}

void GuiServer::pl_prev(){
    const QString cmd[] = {"\"playlist-prev\"",NULL};
    this->send_command(cmd);
}

void GuiServer::load_file(const QString &filename){
    const QString cmd[] = {"\"loadfile\"",quoted(filename), NULL};
    this->send_command(cmd);
}


void GuiServer::send_command(const QString cmd[]){

    QString* json_cmd = new QString();
    int i = 0;
    while (cmd[i] != NULL){
       json_cmd->append(cmd[i++]);
       if (cmd[i] != NULL) json_cmd->append(",");
    }

    json_cmd->prepend("{\"command\":[");
    json_cmd->append("]}\n");
    qDebug() << *json_cmd;
    this->write(json_cmd->toUtf8());
}


