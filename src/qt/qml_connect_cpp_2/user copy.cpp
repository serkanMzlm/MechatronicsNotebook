#include "user.h"

user::user(QString name, uint8_t age):name(name), age(age), number(0)
{
}

void user::changeNumber(){
    QString name_ = QString::number(number);
    setName(name_);
    number++;
    getAge();

}

void user::getAge() const
{
    qDebug()<<age;
}

void user::setName(QString name)
{
    if(this->name == name ) return;

    this->name = name;
    qDebug()<< this->name;
    emit nameChanged();
}

QString user::getName() const
{
    return name;
}


