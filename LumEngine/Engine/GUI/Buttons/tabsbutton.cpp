#include "tabsbutton.h"

TabsButton::TabsButton(QWidget* parent) : BaseButton(parent)
{
    setIcon(QIcon(":/Icons/New/CMEngine_Tabs.svg"));
}

void TabsButton::enterEvent(QEnterEvent *event) {

    setIcon(QIcon(":/Icons/New/CMEngine_TabsHover.svg"));

    QPushButton::enterEvent(event);
}

void TabsButton::leaveEvent(QEvent *event) {

    setIcon(QIcon(":/Icons/New/CMEngine_Tabs.svg"));

    QPushButton::leaveEvent(event);
}