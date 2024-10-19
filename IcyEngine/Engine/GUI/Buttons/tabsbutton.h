#pragma once

#include "basebutton.h"

class TabsButton : public BaseButton
{
public:
    TabsButton(QWidget* parent = nullptr);

protected:
    virtual void enterEvent(QEnterEvent* event);
    virtual void leaveEvent(QEvent* event);

private:
    bool isTabOpen = false;
};