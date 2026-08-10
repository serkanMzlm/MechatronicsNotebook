#include <QCoreApplication>
#include <QVariant>

#include "test.h"
#include "watcher.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Test tester;
    Watcher destination;

    QObject::connect(&tester, &Test::messageChanged, &destination, &Watcher::messageChanged);

    tester.setProperty("message",QVariant("Hello World")); // Direkt setProperty fonksiyonuyla çağırma işlemleri yapılabilir.
    tester.setMessage("testing");
    return a.exec();
}
