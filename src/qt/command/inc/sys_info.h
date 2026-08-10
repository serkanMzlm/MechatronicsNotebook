#ifndef SYS_INFO_H
#define SYS_INFO_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <ctime>

class SysInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString clock READ clock WRITE setClock NOTIFY clockChanged FINAL)
    Q_PROPERTY(QString calender READ calender WRITE setCalender NOTIFY calenderChanged FINAL)
public:
    explicit SysInfo(QObject *parent = nullptr);
    void updateSystemInfo();

    void setClock(const QString &newClock);
    void setCalender(const QString &newCalender);

    QString clock() const;
    QString calender() const;

signals:
    void clockChanged();
    void chargeChanged();
    void calenderChanged();

private:
    std::tm* local_time;
    std::time_t current_time;
    QTimer *timer;

    QString m_clock;
    QString m_calender;
};

#endif // SYS_INFO_H
