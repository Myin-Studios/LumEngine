#pragma once

#include "basebutton.h"

class PlayButton : public BaseButton
{
public:
    PlayButton(QWidget* parent = nullptr);

protected:
    virtual void enterEvent(QEnterEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void mousePressEvent(QMouseEvent *eventPress);

private:
    bool isPlaying = false;
};