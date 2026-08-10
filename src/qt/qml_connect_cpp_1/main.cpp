#include <QGuiApplication>
#include "window.h"

int main(int argc, char *argv[]){
    QGuiApplication app(argc, argv);
    window win;
    win.view.setSource(QUrl("qrc:main.qml"));
    win.view.show();
    return app.exec();
}
