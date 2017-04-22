#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include <QWidget>
#include <QWheelEvent>
#include <QPoint>
#include <iostream>
#include <QPainter>
#include <QPixmap>
#include <QPointF>
#include <QRectF>

#define MAX_VOLUME 130

using namespace std;

class VolumeWidget : public QWidget
{
    Q_OBJECT

private:
    int volume;
    const QPixmap* imgVolume;
    const QPixmap* imgHP;

public:
    explicit VolumeWidget(QWidget *parent = 0);

    void wheelEvent(QWheelEvent *e);
    void paintEvent(QPaintEvent *e);

signals:

public slots:

};

#endif // VOLUMEWIDGET_H
