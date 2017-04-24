#include "mediacontrol.h"

mediaControl::mediaControl(QObject *parent) :
    QObject(parent)
{

    machineMediaControl = new QStateMachine(this);

    Pause = new QState(machineMediaControl);
    Lecture = new QState(machineMediaControl);
    Stop =new QState(machineMediaControl);


    machineMediaControl->setInitialState(Stop);

    connect(Lecture,SIGNAL(entered()),this,SLOT(enteredLecture()));
   // connect(this,SIGNAL(retourLecture()),this,SLOT(lecture()));

    connect(Pause,SIGNAL(entered()),this,SLOT(enteredPause()));
   // connect(this,SIGNAL(retourPause()),this,SLOT(pause()));

    connect(Stop,SIGNAL(entered()),this,SLOT(enteredStop()));

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

    emit(setStop());
}
