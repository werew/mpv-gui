#include "mediacontrol.h"

mediaControl::mediaControl(QObject *parent) :
    QObject(parent)
{

    machineMediaControl = new QStateMachine(this);

    Pause = new QState(machineMediaControl);
    Lecture = new QState(machineMediaControl);
    SuivantP = new QState(machineMediaControl);
    PrecedentP = new QState(machineMediaControl);
    SuivantL = new QState(machineMediaControl);
    PrecedentL = new QState(machineMediaControl);

    machineMediaControl->setInitialState(Pause);

    connect(Lecture,SIGNAL(entered()),this,SLOT(lecture()));
   // connect(this,SIGNAL(retourLecture()),this,SLOT(lecture()));

    connect(Pause,SIGNAL(entered()),this,SLOT(pause()));
   // connect(this,SIGNAL(retourPause()),this,SLOT(pause()));

    connect(SuivantL,SIGNAL(entered()),this,SLOT(suivantL()));
    connect(PrecedentL,SIGNAL(entered()),this,SLOT(precedentL()));

    connect(SuivantP,SIGNAL(entered()),this,SLOT(suivantP()));
    connect(PrecedentP,SIGNAL(entered()),this,SLOT(precedentP()));


}

void mediaControl::lecture()
{
    cout<<"lecture"<<endl;
    emit(setPlay());
}

void mediaControl::pause()
{
    cout << "pause"<<endl;
    emit(setPause());
}

void mediaControl::suivantL()
{
    cout << "suivant" << endl;

    emit(retour());
}

void mediaControl::precedentL()
{
    cout << "precedent" << endl;

    emit(retour());
}

void mediaControl::suivantP()
{
    cout << "suivant" << endl;

    emit(retour());
}

void mediaControl::precedentP()
{
    cout << "precedent" << endl;

    emit(retour());
}

