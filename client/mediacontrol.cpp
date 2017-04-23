#include "mediacontrol.h"

mediaControl::mediaControl(QObject *parent) :
    QObject(parent)
{

    machineMediaControl = new QStateMachine(this);

    Pause = new QState(machineMediaControl);
    Lecture = new QState(machineMediaControl);
    Stop =new QState(machineMediaControl);
    SuivantP = new QState(machineMediaControl);
    PrecedentP = new QState(machineMediaControl);
    SuivantL = new QState(machineMediaControl);
    PrecedentL = new QState(machineMediaControl);
    PrecedentS = new QState(machineMediaControl);
    SuivantS = new QState(machineMediaControl);

    machineMediaControl->setInitialState(Stop);

    connect(Lecture,SIGNAL(entered()),this,SLOT(enteredLecture()));
   // connect(this,SIGNAL(retourLecture()),this,SLOT(lecture()));

    connect(Pause,SIGNAL(entered()),this,SLOT(enteredPause()));
   // connect(this,SIGNAL(retourPause()),this,SLOT(pause()));

    connect(Stop,SIGNAL(entered()),this,SLOT(enteredStop()));

    connect(SuivantL,SIGNAL(entered()),this,SLOT(enteredSuivantL()));
    connect(PrecedentL,SIGNAL(entered()),this,SLOT(enteredPrecedentL()));

    connect(SuivantP,SIGNAL(entered()),this,SLOT(enteredSuivantP()));
    connect(PrecedentP,SIGNAL(entered()),this,SLOT(enteredPrecedentP()));

    connect(SuivantS,SIGNAL(entered()),this,SLOT(enteredSuivantS()));
    connect(PrecedentS,SIGNAL(entered()),this,SLOT(enteredPrecedentS()));

}

void mediaControl::enteredLecture()
{
    lecture();

    emit(setPlay());
}

void mediaControl::enteredPause()
{
    pause();

    emit(setPause());
}

void mediaControl::enteredStop()
{
    stop();

    emit(setStop());
}

void mediaControl::enteredSuivantL()
{
    suivant();

    emit(retour());
}

void mediaControl::enteredPrecedentL()
{
    precedent();

    emit(retour());
}

void mediaControl::enteredSuivantP()
{
    suivant();

    emit(retour());
}

void mediaControl::enteredPrecedentP()
{
    precedent();

    emit(retour());
}

void mediaControl::enteredPrecedentS()
{
    precedent();

    emit(retour());
}

void mediaControl::enteredSuivantS()
{
    suivant();

    emit(retour());
}


void mediaControl::precedent()
{
    cout << "precedent" << endl;
}

void mediaControl::suivant()
{
    cout << "suivant" << endl;
}

void mediaControl::lecture()
{
    cout << "lecture" << endl;
}

void mediaControl::pause()
{
    cout << "pause" << endl;
}

void mediaControl::stop()
{
    cout << "stop" << endl;
}
