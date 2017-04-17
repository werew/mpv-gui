#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QLocalServer>


class Server : public QObject
{
    Q_OBJECT
private:
    QLocalServer *myserver;

public:
    explicit Server(QObject *parent = 0);

signals:
    void finished();

public slots:

};

#endif // SERVER_H
