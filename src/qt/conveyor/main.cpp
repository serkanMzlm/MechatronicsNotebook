#include <QIcon>
#include <QDirIterator>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "serial_comm.h"
#include "date_time.h"
#include "navigation_menu.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/conveyor/assets/icons/conveyor.ico"));
    // app.setWindowIcon(QIcon(":/qt/qml/conveyor/assets/icons/conveyor.ico"));
    QDirIterator qrc(":", QDirIterator::Subdirectories);
    SerialComm* serial = new SerialComm(&app);
    DateTime* date_time = new DateTime(&app);
    NavigationMenu* navigation_menu = new NavigationMenu(&app);
    QQmlApplicationEngine engine;
    qmlRegisterSingletonInstance("conveyor.datetime", 1, 0, "DateTime", date_time);
    qmlRegisterSingletonInstance("conveyor.serialcom", 1, 0, "SerialComm", serial);
    qmlRegisterSingletonInstance("conveyor.navigation", 1, 0, "NavigationMenuSection", navigation_menu);
    const QUrl url(u"qrc:/conveyor/qml/Main.qml"_qs);
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
