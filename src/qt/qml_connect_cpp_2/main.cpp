#include <QGuiApplication>
#include <QQmlContext>
#include <QQuickView>
#include <QQmlEngine>
#include <QTimer>
#include "user.h"


int main(int argc, char *argv[]){
    QGuiApplication app(argc, argv);
    QQmlContext* context;
    QQuickView view;
    context = view.engine()->rootContext();
    user usr(0);
    context->setContextProperty("_user", &usr);
    view.setSource(QUrl("qrc:main.qml"));
    view.show();
    return app.exec();
}
