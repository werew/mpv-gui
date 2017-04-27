#ifndef QGUICLIENTSOCKET_H
#define QGUICLIENTSOCKET_H

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
    PERCENT_POS,
    TIME_POS,
    DURATION,
    META,
    VOLUME,
    STEP_FOR,
    STEP_BACK
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

    void config(const QString conf);
    void config(QJsonObject *conf);
    void pause();
    void unpause();
    void meta(const QString meta);
    void meta(QJsonObject *meta);
    void load(const QString file);
    void percent_pos(double pos);
    void time_pos(double pos);
    void volume(int value);
    void duration(double value);
    void stop();
    QString jsonToString(QJsonObject *o);

private slots:
    void signal_readyRead();
    void signal_disconnected();

};

#endif // QGUICLIENTSOCKET_H
