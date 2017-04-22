#include "volumewidget.h"

VolumeWidget::VolumeWidget(QWidget *parent) :
    QWidget(parent)
{
    volume = 50;
    imgVolume = new QPixmap(":/images/images/gradient_tirangle.png");
    imgHP = new QPixmap(":/images/images/HP.png");
    this->setMinimumHeight(40);
    this->setMinimumWidth(170);
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

    painter.drawPixmap(10,10,20,25,*imgHP);

    QPointF* pt = new QPointF(40+volume/2,20);
    QRectF* rect = new QRectF(0,0,volume,40);
    QPainter::PixmapFragment fragment = QPainter::PixmapFragment::create(*pt,*rect,1,1,0,1);
    painter.drawPixmapFragments(&fragment,10,*imgVolume);
}
