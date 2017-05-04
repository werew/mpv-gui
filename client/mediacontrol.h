//Author : CONGILIO Luigi    CONSTANS Victor

#ifndef MEDIACONTROL_H
#define MEDIACONTROL_H

#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QFinalState>
#include <QSignalTransition>
#include <QTimer>
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

    QTimer* fast_forward_timer;
    QTimer* fast_backward_timer;

    QState* fast_forward_play;
    QState* fast_backward_play;
    QState* fast_forward_pause;
    QState* fast_backward_pause;



public:
    explicit mediaControl(QObject *parent = 0 );

signals:
    void retour();
    void setPause();
    void setPlay();
    void setStop();
    void connectPause();
    void connectPlay();

public slots:
    void enteredLecture();
    void enteredPause();
    void enteredStop();

    void enteredFastForward();
    void enteredFastBackward();
    void exitedFastForward();
    void exitedFastBackward();

private:

};

#endif // MEDIACONTROL_H
