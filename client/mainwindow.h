#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include "mediacontrol.h"
#include "volumewidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QListWidgetItem* items[50];
    mediaControl* mc;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public:
    Ui::MainWindow *ui;

 private slots:
    void itemSelected(QListWidgetItem* it);
};

#endif // MAINWINDOW_H
