#include <QIcon>
#include <QDirIterator>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "date_time.h"
#include "section_navigation_menu.h"
#include "serial_comm.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/qt/qml/conveyor_2/assets/icons/conveyor.ico"));

    SerialComm* serial = new SerialComm(&app);
    DateTime* date_time = new DateTime(&app);

    QQmlApplicationEngine engine;

    qmlRegisterSingletonInstance("conveyor.datetime", 1, 0, "DateTime", date_time);
    qmlRegisterSingletonInstance("conveyor.serialcom", 1, 0, "SerialComm", serial);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("conveyor_2", "Main");

    return app.exec();
}
