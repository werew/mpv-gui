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
    connect(ui->barreLecture,SIGNAL(sliderReleased()),this,SLOT(setBarreLecture()));

    int i;

    for(i=0;i<50;i++)
    {
        items[i] = new QListWidgetItem;
        items[i]->setText(QString::number(i));
        ui->liste->insertItem(i,items[i]);
    }

    mc = new mediaControl();

    mc->Pause->addTransition(ui->lecturePause,SIGNAL(clicked()),mc->Lecture);
    mc->Lecture->addTransition(ui->lecturePause,SIGNAL(clicked()),mc->Pause);
    mc->Lecture->addTransition(ui->suivant,SIGNAL(clicked()),mc->SuivantL);
    mc->Lecture->addTransition(ui->precedent,SIGNAL(clicked()),mc->PrecedentL);
    mc->Pause->addTransition(ui->suivant,SIGNAL(clicked()),mc->SuivantP);
    mc->Pause->addTransition(ui->precedent,SIGNAL(clicked()),mc->PrecedentP);
    mc->PrecedentL->addTransition(mc,SIGNAL(retour()),mc->Lecture);
    mc->SuivantL->addTransition(mc,SIGNAL(retour()),mc->Lecture);
    mc->PrecedentP->addTransition(mc,SIGNAL(retour()),mc->Pause);
    mc->SuivantP->addTransition(mc,SIGNAL(retour()),mc->Pause);

    mc->machineMediaControl->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::itemSelected(QListWidgetItem* it)
{
    cout<<it->text().toStdString()<<endl;
}

void MainWindow::setBarreLecture()
{
   cout << ui->barreLecture->value() <<endl;
}

