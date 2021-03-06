//Author : CONGILIO Luigi    CONSTANS Victor

#ifndef VOLUMEWIDGET_H
#define VOLUMEWIDGET_H

#include <QWidget>
#include <QWheelEvent>
#include <QMouseEvent>
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
    int saveVolume;
    const QPixmap* imgVolume;
    const QPixmap* imgBordVolume;
    const QPixmap* imgHP;
    const QPixmap* imgHP_muet;

public:
    explicit VolumeWidget(QWidget *parent = 0);

    void wheelEvent(QWheelEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

    void setVolume(int volume);

signals:
    void clientChangeVolume(int volume);

public slots:

};

#endif // VOLUMEWIDGET_H
