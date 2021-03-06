//Author : CONGILIO Luigi    CONSTANS Victor

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
#include <qguiclientsocket.h>

#include <fileref.h>
#include <tpropertymap.h>
#include <QMap>
#include <QJsonValueRef>

#define SERVER_NAME "mpvguiserver"
#define MAX_SIZECMD 2048

enum {
    PIECE,
    PLAYLIST,
    RADIO,
    NONE
};



class Server : public QObject
{
    Q_OBJECT
private:
    QLocalServer *myserver;
    QList<QGuiClientSocket*> *clients;
    QMpvSocket *mpv;
    QJsonObject config;
    QMap<QString, QString> loadings;

    // Current state
    int volume;
    double percent_pos;
    double time_pos;
    double duration;
    bool pause;
    bool stop;
    int type_stream;
    QString stream;
    QJsonObject metadata;

    void loadFile_req(QString filename);
    void loadFile_res(QString filename);

    void loadPiece(QString p);
    void loadRadio(QString r);


public:
    explicit Server(QObject *parent, char* configfile);
    void listen();
    void importConfig(const char* filename);
    QJsonObject getTags(QString fileName);

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
    void sendAll(QGuiClientSocket *client);
    void loadList(QString list, int index);
    void loadNext();
    void loadPrevious();

};

#endif // SERVER_H
