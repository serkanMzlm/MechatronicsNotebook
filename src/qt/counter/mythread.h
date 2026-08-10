#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QObject>
#include <QMutex>
#include <QThread>
#include <QtCore>
#include <QDebug>

class myThread: public QThread
{
    Q_OBJECT
public:
    explicit myThread(QObject *param = 0);
    myThread();
    void run();
    bool stop;
    QMutex _mutex;
    QThread* _thread;
signals:
    void valueChange(int);

public slots:
};

#endif // MYTHREAD_H
