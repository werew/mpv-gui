#include "volumewidget.h"

VolumeWidget::VolumeWidget(QWidget *parent) :
    QWidget(parent)
{
    volume = 50;
}

void VolumeWidget::wheelEvent(QWheelEvent *e)
{
    QPoint angle = e->angleDelta()/8;

    cout << "x: " << angle.x() << " y: " << angle.y() << endl;
}

void VolumeWidget::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);

    painter.drawPixmap();
}
