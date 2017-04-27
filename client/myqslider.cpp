#include "myqslider.h"

MyQSlider::MyQSlider(QWidget *parent) :
    QSlider(parent)
{

}

void MyQSlider::mousePressEvent ( QMouseEvent * event )
{
    if (event->button() == Qt::LeftButton)
    {
        setValue(minimum() + (maximum() - minimum()) *
                 (static_cast<float>(event->x()) / static_cast<float>(width())));

        event->accept();
    }
    QSlider::mousePressEvent(event);
}
