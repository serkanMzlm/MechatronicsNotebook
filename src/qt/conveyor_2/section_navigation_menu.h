#ifndef SECTION_NAVIGATION_MENU_H
#define SECTION_NAVIGATION_MENU_H

#include <QObject>

class SectionNavigationMenu : public QObject
{
    Q_OBJECT
public:
    explicit SectionNavigationMenu(QObject *parent = nullptr);

signals:
};

#endif // SECTION_NAVIGATION_MENU_H
