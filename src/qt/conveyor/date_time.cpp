#include "date_time.h"

DateTime::DateTime(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &DateTime::updateCurrentTime);
    timer->start(1000);
}

DateTime::~DateTime()
{
    delete timer;
}

void DateTime::updateCurrentTime() {
    currentTime = std::time(nullptr);
    localTime = std::localtime(&currentTime);
    setFormattedDateTime();
}

void DateTime::printCurrentDateTime() const {
    std::cout << "Current Date and Time: "
              << (localTime->tm_year + 1900) << '-'
              << (localTime->tm_mon + 1) << '-'
              << localTime->tm_mday << ' '
              << localTime->tm_hour << ':'
              << localTime->tm_min << ':'
              << localTime->tm_sec << std::endl;
}

void DateTime::printCurrentDate() const {
    std::cout << "Current Date: "
              << (localTime->tm_year + 1900) << '-'
              << (localTime->tm_mon + 1) << '-'
              << localTime->tm_mday << std::endl;
}

void DateTime::printCurrentTime() const {
    std::cout << "Current Time: "
              << localTime->tm_hour << ':'
              << localTime->tm_min << ':'
              << localTime->tm_sec << std::endl;
}

void DateTime::setFormattedDateTime()
{
    QString formatted_date = QString("%1.%2.%3")
                                 .arg(localTime->tm_mday, 2, 10, QChar('0'))
                                 .arg(localTime->tm_mon + 1, 2, 10, QChar('0'))
                                 .arg(localTime->tm_year + 1900);

    QString formatted_time = QString("%1:%2:%3")
                                 .arg(localTime->tm_hour, 2, 10, QChar('0'))
                                 .arg(localTime->tm_min, 2, 10, QChar('0'))
                                 .arg(localTime->tm_sec, 2, 10, QChar('0'));

    //qDebug() << formatted_date_time;
    setCalender(formatted_date);
    setClock(formatted_time);
}

QString DateTime::calender() const
{
    return m_calender;
}

void DateTime::setCalender(const QString &newCalender)
{
    if (m_calender == newCalender)
        return;
    m_calender = newCalender;
    emit calenderChanged();
}

QString DateTime::clock() const
{
    return m_clock;
}

void DateTime::setClock(const QString &newClock)
{
    if (m_clock == newClock)
        return;
    m_clock = newClock;
    emit clockChanged();
}
