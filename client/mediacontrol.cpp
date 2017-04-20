#include "mediacontrol.h"

mediaControl::mediaControl(QObject *parent) :
    QObject(parent)
{
    machineMediaControl = new QStateMachine(this);

    Pause = new QState(machineMediaControl);
    Lecture = new QState(machineMediaControl);
    Suivant = new QState(machineMediaControl);
    Precedent = new QState(machineMediaControl);

    machineMediaControl->setInitialState(Pause);
}
