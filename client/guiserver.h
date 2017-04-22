#ifndef GUISERVER_H
#define GUISERVER_H

#include <QObject>

class GuiServer : public QObject
{
    Q_OBJECT
public:
    explicit GuiServer(QObject *parent = 0);

signals:

public slots:

};

#endif // GUISERVER_H
