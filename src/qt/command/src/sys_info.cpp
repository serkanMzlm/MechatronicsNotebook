#include "sys_info.h"

SysInfo::SysInfo(QObject *parent)
    : QObject{parent}
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SysInfo::updateSystemInfo);
    timer->start(1000);
}

void SysInfo::updateSystemInfo()
{
    current_time = std::time(nullptr);
    local_time = std::localtime(&current_time);

    QString format_date = QString("%1.%2.%3")
                              .arg(local_time->tm_mday, 2, 10, QChar('0'))
                              .arg(local_time->tm_mon + 1, 2, 10, QChar('0'))
                              .arg(local_time->tm_year + 1900);

    QString format_time = QString("%1:%2")
                              .arg(local_time->tm_hour, 2, 10, QChar('0'))
                              .arg(local_time->tm_min, 2, 10, QChar('0'));

    setCalender(format_date);
    setClock(format_time);
}

void SysInfo::setClock(const QString &newClock)
{
    if (m_clock == newClock)
        return;
    m_clock = newClock;
    emit clockChanged();
}

void SysInfo::setCalender(const QString &newCalender)
{
    if (m_calender == newCalender)
        return;
    m_calender = newCalender;
    emit calenderChanged();
}

QString SysInfo::clock() const { return m_clock; }
QString SysInfo::calender() const { return m_calender; }
