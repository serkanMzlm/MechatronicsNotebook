#include <QGuiApplication>
//#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QTimer>
#include "user.h"


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlContext* context;
    QQuickView view;
    QTimer* timer = new QTimer(&app);
    timer->start(1000);
    context = view.engine()->rootContext();
    user usr("0", 30);
    QObject::connect(timer, &QTimer::timeout, &usr, &user::changeNumber);

    context->setContextProperty("_user", &usr);

    view.setSource(QUrl("qrc:main.qml"));
    view.show();

    return app.exec();
}
