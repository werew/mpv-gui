#ifndef GUISERVER_H
#define GUISERVER_H

#include <QLocalSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <stdexcept>

enum {
    CONFIG,
    PAUSE,
    UNPAUSE,
    LOAD,
    STOP,
    POS,
    META,
    VOLUME
};


class GuiServer : public QLocalSocket
{
    Q_OBJECT
public:
    explicit GuiServer(QObject *parent);
    ~GuiServer();

signals:


public slots:
    void send_command(int type,const QString data);
    void send_command(int type);

    void pause();
    void unpause();
    void load(const QString file);
    void pos(double pos);
    void volume(int value);
    void stop();
    QString jsonToString(QJsonObject *o);


};

#endif // GUISERVER_H
