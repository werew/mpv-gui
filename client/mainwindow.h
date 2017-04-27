#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define MAX_SIZECMD 4096

#include <QJsonParseError>
#include <QJsonObject>
#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include "mediacontrol.h"
#include "volumewidget.h"
#include "guiserver.h"
#include <QSlider>
#include <string>
#include <stdexcept>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QListWidgetItem* items[50];
    mediaControl* mc;
    GuiServer *server;
    void connectToServer(QString servername);
    void handleServerMsg(QJsonObject o);
    void changeCurrentMusic(QJsonObject o);

    QJsonObject config;



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    Ui::MainWindow *ui;

 private slots:
    void itemSelected(QListWidgetItem* it);
    void changeBarreLectureValue(int value);
    void pause();
    void play();
    void stop();
    void readFromServer();
    void connectPause();
    void connectPlay();
    void printList();

signals:
    void lectureBarreValueChanged(double);
    void lectureSelection();
    void moveToPause();
    void moveToPlay();
    void moveToPrev();
    void moveToNext();
    void moveToStop();

};

#endif // MAINWINDOW_H
