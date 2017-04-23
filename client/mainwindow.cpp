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
    connect(ui->barreLecture,SIGNAL(sliderReleased()),this,SLOT(getBarreLecture()));

    int i;

    for(i=0;i<50;i++)
    {
        items[i] = new QListWidgetItem;
        items[i]->setText(QString::number(i));
        ui->liste->insertItem(i,items[i]);
    }



    server = new GuiServer();


    mc = new mediaControl();

    mc->Pause->addTransition(ui->lecturePause,SIGNAL(clicked()),mc->Lecture);
    mc->Lecture->addTransition(ui->lecturePause,SIGNAL(clicked()),mc->Pause);
    mc->Stop->addTransition(ui->lecturePause,SIGNAL(clicked()),mc->Lecture);
    mc->Stop->addTransition(ui->precedent,SIGNAL(clicked()),mc->PrecedentS);
    mc->Stop->addTransition(ui->suivant,SIGNAL(clicked()),mc->SuivantS);
    mc->Lecture->addTransition(ui->suivant,SIGNAL(clicked()),mc->SuivantL);
    mc->Lecture->addTransition(ui->precedent,SIGNAL(clicked()),mc->PrecedentL);
    mc->Lecture->addTransition(ui->stop,SIGNAL(clicked()),mc->Stop);
    mc->Lecture->addTransition(this,SIGNAL(lectureSelection()),mc->Lecture);
    mc->Pause->addTransition(this,SIGNAL(lectureSelection()),mc->Lecture);
    mc->Stop->addTransition(this,SIGNAL(lectureSelection()),mc->Lecture);
    mc->Pause->addTransition(ui->suivant,SIGNAL(clicked()),mc->SuivantP);
    mc->Pause->addTransition(ui->precedent,SIGNAL(clicked()),mc->PrecedentP);
    mc->Pause->addTransition(ui->stop,SIGNAL(clicked()),mc->Stop);
    mc->PrecedentL->addTransition(mc,SIGNAL(retour()),mc->Lecture);
    mc->SuivantL->addTransition(mc,SIGNAL(retour()),mc->Lecture);
    mc->PrecedentP->addTransition(mc,SIGNAL(retour()),mc->Pause);
    mc->SuivantP->addTransition(mc,SIGNAL(retour()),mc->Pause);
    mc->SuivantS->addTransition(mc,SIGNAL(retour()),mc->Stop);
    mc->PrecedentS->addTransition(mc,SIGNAL(retour()),mc->Stop);

    connect(mc,SIGNAL(setPause()),this,SLOT(play()));
    connect(mc,SIGNAL(setPlay()),this,SLOT(pause()));
    connect(mc,SIGNAL(setStop()),this,SLOT(stop()));


    mc->machineMediaControl->start();
    this->connectToServer("/tmp/mpvguiserver");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connectToServer(QString servername){
    if (server != NULL) delete server;

    server = new GuiServer(this);

    connect(server,SIGNAL(readyRead()),this,SLOT(readFromServer()));

    server->connectToServer(servername);
    if (server->waitForConnected() == false)
        throw runtime_error("Failed to connect to the server\n"+
                                 server->errorString().toStdString());
}

void MainWindow::readFromServer(){
    while (true) {
        QByteArray a = server->readLine(MAX_SIZECMD);
        if (a.isEmpty()) break;

        QJsonParseError error;
        QJsonDocument jDoc = QJsonDocument::fromJson(a, &error);
        if (jDoc.isNull()){
            qDebug() << "Parsing error: "+ error.errorString();
            continue;
        }
        QJsonObject jsonObject = jDoc.object();
        handleServerMsg(jsonObject);
    }
}

void MainWindow::handleServerMsg(QJsonObject o){

}

void MainWindow::itemSelected(QListWidgetItem* it)
{
    cout<<it->text().toStdString()<<endl;
    emit(lectureSelection());
}

void MainWindow::getBarreLecture()
{
   cout << ui->barreLecture->value() <<endl;
}

void MainWindow::setBarreLecture(int value)
{
    ui->barreLecture->setValue(value);
}

void MainWindow::stop()
{
    ui->lecturePause->setIcon(QPixmap(":/images/images/play.png"));
    ui->lecturePause->setIconSize(QSize(40,16));
}

void MainWindow::play()
{
    ui->lecturePause->setIcon(QPixmap(":/images/images/play.png"));
    ui->lecturePause->setIconSize(QSize(40,16));
}

void MainWindow::pause()
{
    ui->lecturePause->setIcon(QPixmap(":/images/images/pause.png"));
    ui->lecturePause->setIconSize(QSize(40,16));

}
