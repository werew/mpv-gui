#ifndef CLIENT_H
#define CLIENT_H

#include <stdexcept>
#include <QObject>
#include <QLocalServer>
#include <QFuture>
#include <QtConcurrent>
#include <QLocalSocket>
#include <QList>
#include <QDebug>
#include "guiserver.h"

#define SERVER_NAME "mpvguiserver"
#define MAX_SIZECMD 2048

class Client : public QObject
{
    Q_OBJECT

private:
    QLocalServer *myserver;
    QList<QLocalSocket*> *clients;
    GuiServer *server;
    QJsonObject config;

    // Current state
    int volume;
    double percent_pos;
    bool pause;
    bool stop;
    QString filename;

public:
    explicit Client(QObject *parent, char* configfile);
    void listen();
    void importConfig(const char* filename);

signals:
    void finished();

public slots:

private slots:
    void handleConnection();
    void bindProperties();
    void readFromClient();
    void removeClient();
    void readFromMpv();
    void handleMpvMsg(QJsonObject o);
    void handleClientMsg(QJsonObject o);

};

#endif // CLIENT_H
