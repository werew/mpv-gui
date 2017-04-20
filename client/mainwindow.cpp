#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->liste,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(itemSelected(QListWidgetItem*)));


    int i;

    for(i=0;i<50;i++)
    {
        items[i] = new QListWidgetItem;
        items[i]->setText(QString::number(i));
        ui->liste->insertItem(i,items[i]);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::itemSelected(QListWidgetItem* it)
{
    cout<<it->text().toStdString()<<endl;
}

