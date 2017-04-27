#ifndef MYQSLIDER_H
#define MYQSLIDER_H

#include <QMouseEvent>
#include <QSlider>

class MyQSlider : public QSlider
{
    Q_OBJECT
public:
    explicit MyQSlider(QWidget *parent = 0);

signals:

public slots:
        void mousePressEvent ( QMouseEvent * event );

};

#endif // MYQSLIDER_H
