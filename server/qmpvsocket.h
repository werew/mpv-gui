#ifndef QMPVSOCKET_H
#define QMPVSOCKET_H

#include <QLocalSocket>

class QMpvSocket : public QLocalSocket
{
    Q_OBJECT
public:
    explicit QMpvSocket(QObject *parent = 0);
    QString quoted(const QString &s);

signals:

public slots:
    void send_command(const QString cmd[]);
    void observe_property(int id, const QString &prop);
    void step_forward();
    void step_backward();

};

#endif // QMPVSOCKET_H
