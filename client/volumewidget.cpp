#include "volumewidget.h"

VolumeWidget::VolumeWidget(QWidget *parent) :
    QWidget(parent)
{
    volume = 50;
    imgVolume = new QPixmap(":/images/images/gradient.png");
}

void VolumeWidget::wheelEvent(QWheelEvent *e)
{
    QPoint angle = e->angleDelta()/8;

    if(volume+angle.y() <= 0)
    {
        volume = 0;
    }
    else if(volume+angle.y() >= MAX_VOLUME)
    {
        volume = MAX_VOLUME;
    }
    else
    {
        volume+=angle.y();
    }
    cout << volume << endl;
    this->repaint();
}

void VolumeWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);

    QPointF* pt = new QPointF(0.0,0.0);
    QRectF* rect = new QRectF(0,0,(float)volume,70);
    QPainter::PixmapFragment fragment = QPainter::PixmapFragment::create(*pt,*rect);
    painter.drawPixmapFragments(&fragment,10,*imgVolume);
}
