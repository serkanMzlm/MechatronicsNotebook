#ifndef NAVIGATION_MENU_H
#define NAVIGATION_MENU_H

#include <QObject>
#include <QDebug>

class NavigationMenu : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int section READ section WRITE setSection NOTIFY sectionChanged FINAL)
public:
    enum {
        HOME,
        ALARMS,
        SETTINGS,
        SERVICE,
        RESET,
        START,
        TEST_MODE,
    };
    explicit NavigationMenu(QObject *parent = nullptr);
    int section() const;
    void setSection(int newSection);

signals:
    void sectionChanged();
private:
    int m_section;
};

#endif // NAVIGATION_MENU_H
