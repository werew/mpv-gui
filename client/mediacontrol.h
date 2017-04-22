#ifndef MEDIACONTROL_H
#define MEDIACONTROL_H

#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QFinalState>
#include <QSignalTransition>
#include <iostream>

using namespace std;


class mediaControl : public QObject
{
    Q_OBJECT

private:
    enum etat{pau,lect};

public:
    QStateMachine* machineMediaControl;

    QState* Pause;
    QState* Lecture;
    QState* SuivantP;
    QState* PrecedentP;
    QState* SuivantL;
    QState* PrecedentL;


public:
    explicit mediaControl(QObject *parent = 0 );

signals:
    void retour();
    void setPause();
    void setPlay();

public slots:
    void lecture();
    void pause();
    void suivantL();
    void precedentL();
    void suivantP();
    void precedentP();

};

#endif // MEDIACONTROL_H
