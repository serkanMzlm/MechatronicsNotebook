#include "agecalc.h"

AgeCalc::AgeCalc(QObject *parent)
{

}

int AgeCalc::age() const
{
    return m_age;
}

void AgeCalc::setAge(int newAge)
{
    m_age = newAge;
}

int AgeCalc::dogYears() const
{
    return this->m_age / 7;
}

int AgeCalc::catYears() const
{
    return this->m_age / 9;
}

int AgeCalc::humanYears() const
{
    return this->m_age;
}

void AgeCalc::print() const
{
    qInfo() << "Human: " << this->humanYears();
    qInfo() << "Cat: " << this->catYears();
    qInfo() << "Dog: " << this->dogYears();
}
