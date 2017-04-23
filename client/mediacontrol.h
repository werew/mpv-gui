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
    QState* Stop;
    QState* SuivantP;
    QState* PrecedentP;
    QState* SuivantL;
    QState* PrecedentL;
    QState* PrecedentS;
    QState* SuivantS;


public:
    explicit mediaControl(QObject *parent = 0 );

signals:
    void retour();
    void setPause();
    void setPlay();
    void setStop();

public slots:
    void enteredLecture();
    void enteredPause();
    void enteredStop();
    void enteredSuivantL();
    void enteredPrecedentL();
    void enteredSuivantP();
    void enteredPrecedentP();
    void enteredPrecedentS();
    void enteredSuivantS();

private:
    void suivant();
    void precedent();
    void lecture();
    void pause();
    void stop();

};

#endif // MEDIACONTROL_H
