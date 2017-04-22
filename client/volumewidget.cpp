#include "volumewidget.h"

VolumeWidget::VolumeWidget(QWidget *parent) :
    QWidget(parent)
{
    volume = 50;
    imgVolume = new QPixmap(":/images/images/gradient_tirangle.png");
    imgHP = new QPixmap(":/images/images/HP.png");
    this->setMinimumHeight(0);
    this->setMinimumWidth(0);
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

    painter.drawPixmap(0,0,20,20,*imgHP);

    QPointF* pt = new QPointF(25+volume/2,0);
    QRectF* rect = new QRectF(0,0,volume,40);
    QPainter::PixmapFragment fragment = QPainter::PixmapFragment::create(*pt,*rect,1,1,0,1);
    painter.drawPixmapFragments(&fragment,10,*imgVolume);
}
