#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QLocalServer>
#include <QFuture>
#include <QtConcurrent>
#include <QDebug>



class Server : public QObject
{
    Q_OBJECT
private:
    bool running;
    QFuture<void> loopThread;
    QLocalServer *myserver;

public:
    explicit Server(QObject *parent = 0);
    void clientLoop(void);

signals:
    void finished();

public slots:

};

#endif // SERVER_H
