#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    Ui::MainWindow *ui;

 private slots:
    void itemSelected(QListWidgetItem* it);
    void setBarreLecture();
    void pause();
    void play();
    void stop();

 private slots:
    void readFromServer();

};

#endif // MAINWINDOW_H
