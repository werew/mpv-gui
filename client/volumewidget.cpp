#include "volumewidget.h"

VolumeWidget::VolumeWidget(QWidget *parent) :
    QWidget(parent)
{
    volume = 50;
    imgVolume = new QPixmap(":/images/volume.png");
}

void VolumeWidget::wheelEvent(QWheelEvent *e)
{
    QPoint angle = e->angleDelta()/8;

    cout << "x: " << angle.x() << " y: " << angle.y() << endl;
}

void VolumeWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);

   // painter.drawPixmap(10,10,50,50,imgVolume);
}
