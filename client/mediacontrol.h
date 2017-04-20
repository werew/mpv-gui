#ifndef MEDIACONTROL_H
#define MEDIACONTROL_H

#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QFinalState>
#include <QSignalTransition>

class mediaControl : public QObject
{
    Q_OBJECT

private:
    QStateMachine* machineMediaControl;

    QState* Pause;
    QState* Lecture;
    QState* Suivant;
    QState* Precedent;

public:
    explicit mediaControl(QObject *parent = 0);

signals:

public slots:

};

#endif // MEDIACONTROL_H
