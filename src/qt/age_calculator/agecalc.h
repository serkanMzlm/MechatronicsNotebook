#ifndef AGECALC_H
#define AGECALC_H

#include <QObject>
#include <QDebug>

class AgeCalc: public QObject
{
    Q_OBJECT
public:
    AgeCalc(QObject *parent = nullptr);

    int age() const;
    void setAge(int newAge);

    int dogYears() const;
    int catYears() const;
    int humanYears() const;
    void print() const;

private:
    int m_age;
    QString m_name;
};

#endif // AGECALC_H
