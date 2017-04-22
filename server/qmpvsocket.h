#ifndef QMPVSOCKET_H
#define QMPVSOCKET_H

#include <QLocalSocket>

class QMpvSocket : public QLocalSocket
{
    Q_OBJECT
public:
    explicit QMpvSocket(QObject *parent = 0);

signals:

public slots:
    void send_command(const QString cmd[]);
    void observe_property(int id, const QString &prop);
    QString quoted(const QString &s);

};

#endif // QMPVSOCKET_H
