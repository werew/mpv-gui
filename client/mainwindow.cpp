//Author : CONGILIO Luigi    CONSTANS Victor

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = NULL;

    mc = new mediaControl(this);

    mc->Pause->addTransition(this,SIGNAL(moveToPlay()),mc->Lecture);
    mc->Lecture->addTransition(this,SIGNAL(moveToPause()),mc->Pause);
    mc->Stop->addTransition(this,SIGNAL(moveToPlay()),mc->Lecture);
    mc->Lecture->addTransition(this,SIGNAL(moveToStop()),mc->Stop);
    /*
    mc->Lecture->addTransition(this,SIGNAL(lectureSelection()),mc->Lecture);
    mc->Pause->addTransition(this,SIGNAL(lectureSelection()),mc->Lecture);
    mc->Stop->addTransition(this,SIGNAL(lectureSelection()),mc->Lecture);
    */
    mc->Pause->addTransition(this,SIGNAL(moveToStop()),mc->Stop);
    mc->Lecture->addTransition(ui->fast_forward,SIGNAL(pressed()),mc->fast_forward_play);
    mc->Lecture->addTransition(ui->fast_backward,SIGNAL(pressed()),mc->fast_backward_play);
    mc->Pause->addTransition(ui->fast_backward,SIGNAL(pressed()),mc->fast_backward_pause);
    mc->Pause->addTransition(ui->fast_forward,SIGNAL(pressed()),mc->fast_forward_pause);

    mc->fast_forward_play->addTransition(ui->fast_forward,SIGNAL(released()),mc->Lecture);
    mc->fast_backward_play->addTransition(ui->fast_backward,SIGNAL(released()),mc->Lecture);
    mc->fast_backward_pause->addTransition(ui->fast_backward,SIGNAL(released()),mc->Pause);
    mc->fast_forward_pause->addTransition(ui->fast_forward,SIGNAL(released()),mc->Pause);


    connect(mc,SIGNAL(setPause()),this,SLOT(play()));
    connect(mc,SIGNAL(setPlay()),this,SLOT(pause()));
    connect(mc,SIGNAL(setStop()),this,SLOT(stop()));

    connect(ui->morceaux,SIGNAL(clicked()),this,SLOT(selectList()));
    connect(ui->listes,SIGNAL(clicked()),this,SLOT(selectList()));
    connect(ui->radios,SIGNAL(clicked()),this,SLOT(selectList()));


    connect(mc,SIGNAL(connectPause()),this,SLOT(connectPause()));
    connect(mc,SIGNAL(connectPlay()),this,SLOT(connectPlay()));

    connect(ui->list_playlists,SIGNAL(itemClicked(QListWidgetItem*)),
            this,SLOT(updatePlaylistItems(QListWidgetItem*)));
    connect(ui->list_playlist_items,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,SLOT(loadFromPlaylist(QListWidgetItem*)));
    connect(ui->liste_morceaux,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,SLOT(load(QListWidgetItem*)));
    connect(ui->list_radios,SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this,SLOT(load(QListWidgetItem*)));

    connect(ui->barreLecture,SIGNAL(valueChanged(int)),this,SLOT(changeBarreLectureValue(int)));


    connect(ui->chgToFr,SIGNAL(triggered()),this,SLOT(changeToFrench()));
    connect(ui->chgToEn,SIGNAL(triggered()),this,SLOT(changeToEnglish()));
    connect(ui->chgToIt,SIGNAL(triggered()),this,SLOT(changeToItalian()));

    mc->machineMediaControl->start();
    this->connectToServer("/tmp/mpvguiserver");

    QString langueSystem = QLocale::system().name().section('_',0,0);

    langue = new QTranslator();

    langue->load(QString("client_")+langueSystem);
    qApp->installTranslator(langue);

    ui->retranslateUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::changeToFrench()
{
    changeLanguage("fr");
}

void MainWindow::changeToEnglish()
{
    changeLanguage("en");
}

void MainWindow::changeToItalian()
{
    changeLanguage("it");
}

void MainWindow::changeLanguage(QString newLangue)
{
    qApp->removeTranslator(langue);

    langue->load(QString("client_")+newLangue);
    qApp->installTranslator(langue);

    ui->retranslateUi(this);
}


void MainWindow::connectToServer(QString servername){
    if (server != NULL)
        delete server;

    server = new GuiServer(this);

    connect(server,SIGNAL(readyRead()),this,SLOT(readFromServer()));

    connect(ui->lecturePause,SIGNAL(clicked()),server,SLOT(unpause()));
    connect(ui->stop,SIGNAL(clicked()),server,SLOT(stop()));

    connect(ui->volume,SIGNAL(clientChangeVolume(int)),server,SLOT(volume(int)));

    connect(this,SIGNAL(lectureBarreValueChanged(double)),server,SLOT(percent_pos(double)));

    connect(ui->suivant,SIGNAL(clicked()),server,SLOT(next()));
    connect(ui->precedent,SIGNAL(clicked()),server,SLOT(prev()));
    connect(mc->fast_forward_timer,SIGNAL(timeout()),server,SLOT(step_forward()));
    connect(mc->fast_backward_timer,SIGNAL(timeout()),server,SLOT(step_backward()));

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
            qDebug() << a << endl;
            continue;
        }
        QJsonObject jsonObject = jDoc.object();
        handleServerMsg(jsonObject);
    }
}

void MainWindow::handleServerMsg(QJsonObject o){
    QJsonObject meta;
    switch (o["type"].toInt()){
        case VOLUME: ui->volume->setVolume(o["data"].toInt());
          break;
        case PERCENT_POS:
              if(ui->barreLecture->isSliderDown()) return;
              // Break update loop
              disconnect(ui->barreLecture,SIGNAL(valueChanged(int)),
                         this,SLOT(changeBarreLectureValue(int)));
              ui->barreLecture->setValue((int)(o["data"].toDouble()*10));
              // Restore update loop
              connect(ui->barreLecture,SIGNAL(valueChanged(int)),
                      this,SLOT(changeBarreLectureValue(int)));
              break;
        case PAUSE:
                    qDebug() << "Pause";
                    emit(moveToPause());
              break;
        case UNPAUSE:
                    qDebug() << "UnPause";
                    emit(moveToPlay());
              break;
        case STOP:
                    emit(moveToStop());
              break;
        case LOAD:
                changeCurrentMusic(o);
              break;
        case TIME_POS: ui->tempsCourant->setText(QString().sprintf(
                            "%d:%02d",
                            (int)o["data"].toDouble() / 60,
                            (int)o["data"].toDouble() % 60
                           )
                        );
                break;
        case DURATION: ui->tempsTotal->setText(QString().sprintf(
                            "%d:%02d",
                            (int)o["data"].toDouble() / 60,
                            (int)o["data"].toDouble() % 60
                           )
                        );
            break;
        case META:
                meta = o["data"].toObject();

                if(meta.contains("icy-name"))
                {
                    ui->author->setText(meta["icy-name"].toString());
                    ui->trackName->setText(meta["icy-title"].toString());
                    ui->album->setText(meta["icy-genre"].toString());
                }
                else
                {
                    ui->trackName->setText(meta["TITLE"].toString());
                    ui->author->setText(meta["ARTIST"].toString());
                    ui->album->setText(meta["ALBUM"].toString());
                }
            break;
        case CONFIG:
                config = o["data"].toObject();
                updateSelections();
            break;

    }
}

void MainWindow::updateSelections(){
    QStringList l = config["Pieces"].toObject().keys();
    ui->liste_morceaux->clear();
    ui->liste_morceaux->addItems(l);

    l = config["Playlists"].toObject().keys();
    ui->list_playlists->clear();
    ui->list_playlist_items->clear();
    ui->list_playlists->addItems(l);

    l = config["Radios"].toObject().keys();
    ui->list_radios->clear();
    ui->list_radios->addItems(l);
}


void MainWindow::updatePlaylistItems(QListWidgetItem* i){
  QJsonObject o = config["Playlists"].toObject();
  QJsonObject list = o[i->text()].toObject();
  _currentPlaylist = i->text();
  ui->list_playlist_items->clear();
  ui->list_playlist_items->addItems(list.keys());
}

void MainWindow::load(QListWidgetItem* i){
   //QJsonObject o;
   //QString path;
   if (i->listWidget() == ui->liste_morceaux)
       server->loadPiece(i->text());
     //  o = config["Pieces"].toObject();
   else
       server->loadRadio(i->text());
     //  o = config["Radios"].toObject();
   server->unpause();

   //path = o[i->text()].toString();
   //server->load(path);
}

void MainWindow::loadFromPlaylist(QListWidgetItem* i){
   int nb = i->listWidget()->row(i);
   server->loadPlaylist(_currentPlaylist, nb);
   server->unpause();
}

void MainWindow::selectList(){
    if (ui->morceaux->isChecked()){
       ui->listStack->setCurrentIndex(0);
    } else if (ui->listes->isChecked()) {
       ui->listStack->setCurrentIndex(1);
    } else {
       ui->listStack->setCurrentIndex(2);
    }
}

void MainWindow::printList(){
    // TODO try empty config
  /*  QStringList l;
    if (ui->morceaux->isChecked()){
       l = config["Pieces"].toObject().keys();
       ui->listStack->setCurrentIndex(2);
       ui->liste->clear();
    } else if (ui->listes->isChecked()) {
       l = config["Playlists"].toObject().keys();
       ui->listStack->setCurrentIndex(1);
    } else {
       l = config["Radios"].toObject().keys();
       ui->listStack->setCurrentIndex(2);
    }
    ui->liste->addItems(l);
    */
}

void MainWindow::itemSelected(QListWidgetItem* it)
{
    cout<<it->text().toStdString()<<endl;
    emit(lectureSelection());
}

void MainWindow::changeBarreLectureValue(int value)
{
    double val = ((double)(value)/10.0);
    emit(lectureBarreValueChanged(val));
}

void MainWindow::changeCurrentMusic(QJsonObject o)
{
    Q_UNUSED(o);
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
