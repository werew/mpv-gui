//Author : CONGILIO Luigi    CONSTANS Victor

#include "mediacontrol.h"

mediaControl::mediaControl(QObject *parent) :
    QObject(parent)
{

    machineMediaControl = new QStateMachine(this);

    Pause = new QState(machineMediaControl);
    Lecture = new QState(machineMediaControl);
    Stop =new QState(machineMediaControl);
    fast_forward_play = new QState(machineMediaControl);
    fast_forward_pause = new QState(machineMediaControl);
    fast_backward_play = new QState(machineMediaControl);
    fast_backward_pause = new QState(machineMediaControl);


    fast_forward_timer = new QTimer(this);
    fast_backward_timer = new QTimer(this);
    fast_forward_timer->setInterval(100);
    fast_backward_timer->setInterval(100);



    machineMediaControl->setInitialState(Stop);

    connect(Lecture,SIGNAL(entered()),this,SLOT(enteredLecture()));
   // connect(this,SIGNAL(retourLecture()),this,SLOT(lecture()));

    connect(Pause,SIGNAL(entered()),this,SLOT(enteredPause()));
   // connect(this,SIGNAL(retourPause()),this,SLOT(pause()));

    connect(Stop,SIGNAL(entered()),this,SLOT(enteredStop()));
    connect(fast_forward_play,SIGNAL(entered()),this,SLOT(enteredFastForward()));
    connect(fast_forward_play,SIGNAL(exited()),this,SLOT(exitedFastForward()));
    connect(fast_backward_play,SIGNAL(entered()),this,SLOT(enteredFastBackward()));
    connect(fast_backward_play,SIGNAL(exited()),this,SLOT(exitedFastBackward()));

    connect(fast_forward_pause,SIGNAL(entered()),this,SLOT(enteredFastForward()));
    connect(fast_forward_pause,SIGNAL(exited()),this,SLOT(exitedFastForward()));
    connect(fast_backward_pause,SIGNAL(entered()),this,SLOT(enteredFastBackward()));
    connect(fast_backward_pause,SIGNAL(exited()),this,SLOT(exitedFastBackward()));

}

void mediaControl::enteredLecture()
{
    emit(connectPause());

    emit(setPlay());
}

void mediaControl::enteredPause()
{
    emit(connectPlay());

    emit(setPause());
}

void mediaControl::enteredStop()
{
    emit(connectPlay());

    emit(setStop());
}

void mediaControl::enteredFastBackward()
{
    fast_backward_timer->start();
}

void mediaControl::enteredFastForward()
{
    fast_forward_timer->start();
}

void mediaControl::exitedFastBackward()
{
    fast_backward_timer->stop();
}

void mediaControl::exitedFastForward()
{
    fast_forward_timer->stop();
}
