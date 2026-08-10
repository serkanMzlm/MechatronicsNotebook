#ifndef USER_H
#define USER_H

#include <QObject>
#include <QDebug>
#include <QTimer>

class user: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString count READ getCount WRITE setCount NOTIFY countChanged)
public:
    user(int count);
    ~user();
    QString getCount() const;
    void setCount(QString count);
    
public slots:
    void changeNumber();
signals:
    void countChanged();

private:
    QString count;
    int count_;
    int size;
    QTimer* timer = new QTimer();
};

#endif // USER_H
