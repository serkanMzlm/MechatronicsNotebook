#include "navigation_menu.h"

NavigationMenu::NavigationMenu(QObject *parent)
    : QObject{parent}
{
    setSection(HOME);
}

int NavigationMenu::section() const
{
    return m_section;
}

void NavigationMenu::setSection(int newSection)
{
    if (m_section == newSection && m_section >= HOME && m_section <= TEST_MODE)
        return;
    m_section = newSection;
    emit sectionChanged();
}
