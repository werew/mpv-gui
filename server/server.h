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

#include <fileref.h>
#include <tpropertymap.h>

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

    // Current state
    int volume;
    double percent_pos;
    bool pause;
    bool stop;
    QString filename;


public:
    explicit Server(QObject *parent, char* configfile);
    void listen();
    void importConfig(const char* filename);
    QMap<QString, QString> getTags(QString fileName);

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

#endif // SERVER_H
