#ifndef WINDOW_H
#define WINDOW_H

#include <QObject>
#include <QQmlContext>
#include <QQmlEngine>
#include <QDateTime>
#include <QQuickView>

class window: public QObject{
    Q_OBJECT
public:
    window();
    ~window();
    QQuickView view;
    QQmlContext* context = view.engine()->rootContext();
};

#endif // WINDOW_H
