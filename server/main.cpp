#include <QCoreApplication>
#include <server.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server *s = new Server(&a);
    QObject::connect(s, SIGNAL(finished()), &a, SLOT(quit()));
    s->listen();
    return a.exec();
}
