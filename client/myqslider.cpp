#include "myqslider.h"

MyQSlider::MyQSlider(QWidget *parent) :
    QSlider(parent)
{

}

void MyQSlider::mousePressEvent ( QMouseEvent * event )
{
    if (event->button() == Qt::LeftButton)
    {
        if (orientation() == Qt::Horizontal)
            setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
        else
            setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() ) ;

        event->accept();
    }
    QSlider::mousePressEvent(event);
}
