#include <QIcon>
#include <QDebug>
#include <QQmlContext>
#include <QDirIterator>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "sys_info.h"
#include "serial_comm.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/medicine.ico"));

    SysInfo* sys_info = new SysInfo(&app);
    SerialComm* comm = new SerialComm(&app);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/Main.qml"));

    qmlRegisterSingletonInstance("lectron.sysinfo", 1, 0, "SysInfo", sys_info);
    qmlRegisterSingletonInstance("lectron.com", 1, 0, "SerialComm", comm);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
