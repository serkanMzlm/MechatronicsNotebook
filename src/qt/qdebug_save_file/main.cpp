#include <QCoreApplication>
#include <QDebug>
#include "logger.h"
#include "test.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qInfo() << "File:" << Logger::filename;
    Logger::attach();

    qInfo() << "test";

    Logger::logging = false;
    qInfo() << "This will not get logged";
    Logger::logging = true;

    Test t;
    t.testing();

    qInfo() << "Finished";

    return a.exec();
}
