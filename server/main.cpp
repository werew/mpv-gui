//Author : CONGILIO Luigi    CONSTANS Victor

#include <QCoreApplication>
#include <server.h>

int main(int argc, char *argv[])
{
    if (argc < 2){
        qDebug() << "usage:" << argv[0] << " <config file>";
        exit(EXIT_FAILURE);
    }

    try {
        QCoreApplication a(argc, argv);
        Server *s = new Server(&a,argv[1]);
        QObject::connect(s, SIGNAL(finished()), &a, SLOT(quit()));
        s->listen();
    return a.exec();
    } catch (std::exception const& e){
        qDebug() << "Error: " << e.what();
        exit(EXIT_FAILURE);
    }

}
