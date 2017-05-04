//Author : CONGILIO Luigi    CONSTANS Victor

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
#include <QLocale>
#include <string>
#include <stdexcept>
#include <QTranslator>


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
    QTranslator* langue;

    void connectToServer(QString servername);
    void handleServerMsg(QJsonObject o);
    void changeCurrentMusic(QJsonObject o);
    void changeLanguage(QString newLangue);
    void majLangage();

    QString _currentPlaylist;
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
    void updateSelections();
    void updatePlaylistItems(QListWidgetItem *i);
    void selectList();
    void load(QListWidgetItem *i);
    void changeToFrench();
    void changeToEnglish();
    void changeToItalian();
    void loadFromPlaylist(QListWidgetItem *i);

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
