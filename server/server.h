#ifndef SERVER_H
#define SERVER_H

#include <stdexcept>
#include <QObject>
#include <QLocalServer>
#include <QFuture>
#include <QtConcurrent>
#include <QLocalSocket>
#include <QList>
#include <QDebug>
#include <qmpvsocket.h>

#define SERVER_NAME "mpvguiserver"
#define MAX_SIZECMD 2048



class Server : public QObject
{
    Q_OBJECT
private:
    QLocalServer *myserver;
    QList<QLocalSocket*> *clients;
    QMpvSocket *mpv;
    QJsonObject config;


public:
    explicit Server(QObject *parent, char* configfile);
    void listen();
    void importConfig(const char* filename);

signals:
    void finished();

public slots:

private slots:
    void handleConnection();
    void readFromClient();
    void removeClient();
    void readFromMpv();

};

#endif // SERVER_H
