#include "qmpvsocket.h"

QMpvSocket::QMpvSocket(QObject *parent) :
    QLocalSocket(parent)
{
}

QString QMpvSocket::quoted(const QString &s){
    return "\""+s+"\"";
}


void QMpvSocket::observe_property(int id, const QString &prop){
    const QString cmd[] = {"\"observe_property\"",
                           QString::number(id),
                           quoted(prop), NULL};
    this->send_command(cmd);
}

void QMpvSocket::send_command(const QString cmd[]){

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


