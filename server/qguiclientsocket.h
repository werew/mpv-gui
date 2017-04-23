#ifndef QGUICLIENTSOCKET_H
#define QGUICLIENTSOCKET_H

#include <QLocalSocket>
#include <stdexcept>

enum {
    CONFIG,
    PAUSE,
    UNPAUSE,
    PLAY,
    STOP,
    POS,
    META,
    VOLUME
};

class QGuiClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit QGuiClientSocket(QObject *parent, QLocalSocket *socket);
    ~QGuiClientSocket();
    QByteArray readLine(qint64 maxSize = 0);

signals:
    void readyRead();
    void disconnected();

private:
    QLocalSocket *socket;

public slots:
    void send_command(int type,const QString data);
    void send_command(int type);

    void config(const QString data);
    void pause();
    void unpause();
    void volume(int value);
    void stop();

private slots:
    void signal_readyRead();
    void signal_disconnected();

};

#endif // QGUICLIENTSOCKET_H
