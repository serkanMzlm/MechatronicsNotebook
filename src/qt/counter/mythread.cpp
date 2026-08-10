#include "mythread.h"

myThread::myThread(QObject *param): QThread(param), stop(false)
{


}


void myThread::run()
{
    for(int i=0; i<1000; i++)
    {
        _mutex.lock();
        if(this->stop)
        {
            break;
        }
        _mutex.unlock();
        this->msleep(100);
        emit valueChange(i);
    }

}
