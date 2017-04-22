#ifndef GUISERVER_H
#define GUISERVER_H

#include <QLocalSocket>

class GuiServer : public QLocalSocket
{
    Q_OBJECT
public:
    explicit GuiServer(QObject *parent = 0);
    QString quoted(const QString &s);

signals:

public slots:
    void send_command(const QString cmd[]);
    void observe_property(int id, const QString &prop);
    void set_property(const QString &prop, const QString &value);
    void set_property(const QString &prop, int value);
    void set_property(const QString &prop, bool value);
    void step_forward();
    void step_backward();
    void pause(bool value);
    void volume(int value);
    void stop();
    void pl_next();
    void pl_prev();
    void load_file(const QString &filename);

};

#endif // GUISERVER_H
