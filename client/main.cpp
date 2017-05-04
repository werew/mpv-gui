//Author : CONGILIO Luigi    CONSTANS Victor

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    } catch (std::exception const& e){
        qDebug() << "Error: " << e.what();
        exit(EXIT_FAILURE);
    }
}
