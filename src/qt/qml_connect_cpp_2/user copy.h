#ifndef USER_H
#define USER_H
#include <QObject>
#include <QDebug>

class user: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
public:
    user(QString name, uint8_t age);
    QString getName() const;
    void setName(QString name);
    int number;
    void getAge() const;
    
public slots:
    void changeNumber();
signals:
    void nameChanged();

private:
    QString name;
    int size;
    uint8_t age;

};

#endif // USER_H
