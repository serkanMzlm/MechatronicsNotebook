#include "user.h"

user::user(int count): count_(count){
    timer->start(1000);
    connect(timer, &QTimer::timeout, this, &user::changeNumber);
}

user::~user(){
    delete timer;
}

void user::changeNumber(){
    setCount(QString::number(count_));
    count_++;
}

void user::setCount(QString count){
    if(this->count == count) return;

    this->count = count;
    qDebug()<< this->count;
    emit countChanged();
}

QString user::getCount() const {  return count; }
