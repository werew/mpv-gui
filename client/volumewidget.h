#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include <QWidget>
#include <QWheelEvent>
#include <QPoint>
#include <iostream>
#include <QPainter>
#include <QPixmap>

using namespace std;

class VolumeWidget : public QWidget
{
    Q_OBJECT

private:
    int volume;
    const QPixmap* imgVolume;

public:
    explicit VolumeWidget(QWidget *parent = 0);

    void wheelEvent(QWheelEvent *e);
    void paintEvent(QPaintEvent *e);

signals:

public slots:

};

#endif // VOLUMEWIDGET_H
