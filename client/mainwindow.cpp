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


    mc = new mediaControl();

    mc->Pause->addTransition(this,SIGNAL(moveToPlay()),mc->Lecture);
    mc->Lecture->addTransition(this,SIGNAL(moveToPause()),mc->Pause);
    mc->Stop->addTransition(this,SIGNAL(moveToPlay()),mc->Lecture);
    mc->Lecture->addTransition(this,SIGNAL(moveToStop()),mc->Stop);
    mc->Lecture->addTransition(this,SIGNAL(lectureSelection()),mc->Lecture);
    mc->Pause->addTransition(this,SIGNAL(lectureSelection()),mc->Lecture);
    mc->Stop->addTransition(this,SIGNAL(lectureSelection()),mc->Lecture);
    mc->Pause->addTransition(this,SIGNAL(moveToStop()),mc->Stop);


    connect(mc,SIGNAL(setPause()),this,SLOT(play()));
    connect(mc,SIGNAL(setPlay()),this,SLOT(pause()));
    connect(mc,SIGNAL(setStop()),this,SLOT(stop()));



    connect(mc,SIGNAL(connectPause()),this,SLOT(connectPause()));
    connect(mc,SIGNAL(connectPlay()),this,SLOT(connectPlay()));

    connect(ui->barreLecture,SIGNAL(sliderReleased()),this,SLOT(changeBarreLectureValue(int)));


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

    connect(ui->lecturePause,SIGNAL(clicked()),server,SLOT(unpause()));
    connect(ui->stop,SIGNAL(clicked()),server,SLOT(stop()));

    connect(ui->volume,SIGNAL(clientChangeVolume(int)),server,SLOT(volume(int)));


    connect(this,SIGNAL(lectureBarreValueChanged(double)),server,SLOT(percent_pos(double)));

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
    switch (o["type"].toInt()){
        case VOLUME: ui->volume->setVolume(o["data"].toInt());
        cout << o["data"].toInt() << endl;
          break;
        case PERCENT_POS: ui->barreLecture->setValue((int)(o["data"].toDouble()*10));
              break;
        case PAUSE:
                    emit(moveToPause());
              break;
        case UNPAUSE:
                    emit(moveToPlay());
              break;
        case STOP:
                    emit(moveToStop());
              break;
        case LOAD:
                changeCurrentMusic(o);
        /*case META: QJsonObject m = o["data"].toObject();
                // Use mpv's metadatas only for radios
                if (m.contains("icy-name")){
                    metadata = m;
               QJsonDocument d = QJsonDocument(metadata);
               qDebug() << d.toJson();
                }
              break;
              */
    }
}

void MainWindow::itemSelected(QListWidgetItem* it)
{
    cout<<it->text().toStdString()<<endl;
    emit(lectureSelection());
}

void MainWindow::changeBarreLectureValue()
{
    double val = (double)(ui->barreLecture->value()/10);
    emit(lectureBarreValueChanged(val));
}

void MainWindow::changeCurrentMusic(QJsonObject o)
{
    //TODO
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

void MainWindow::connectPause()
{
    disconnect(ui->lecturePause,SIGNAL(clicked()),server,SLOT(unpause()));
    connect(ui->lecturePause,SIGNAL(clicked()),server,SLOT(pause()));
}

void MainWindow::connectPlay()
{
    disconnect(ui->lecturePause,SIGNAL(clicked()),server,SLOT(pause()));
    connect(ui->lecturePause,SIGNAL(clicked()),server,SLOT(unpause()));
}
