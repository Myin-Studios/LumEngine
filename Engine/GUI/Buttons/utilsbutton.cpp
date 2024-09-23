#include "utilsbuttons.h"

PlayButton::PlayButton(QWidget* parent) : BaseButton(parent)
{
    setIcon(QIcon(":/Icons/New/CMEngine_Play"));
}

void PlayButton::enterEvent(QEnterEvent *event) {

    if (!isPlaying)
        setIcon(QIcon(":/Icons/New/CMEngine_PlayHover"));
    else setIcon(QIcon(":/Icons/New/CMEngine_PauseHover"));

//    setIconSize(QSize(this->width() + 5, this->height() + 5));

    QPushButton::enterEvent(event);
}

void PlayButton::leaveEvent(QEvent *event) {

    if (!isPlaying)
        setIcon(QIcon(":/Icons/New/CMEngine_Play"));
    else setIcon(QIcon(":/Icons/New/CMEngine_Pause"));

//    setIconSize(QSize(this->width() - 5, this->height() - 5));

    QPushButton::leaveEvent(event);
}

void PlayButton::mousePressEvent(QMouseEvent *eventPress)
{
    isPlaying = !isPlaying;

    if (!isPlaying)
        setIcon(QIcon(":/Icons/New/CMEngine_Play"));
    else setIcon(QIcon(":/Icons/New/CMEngine_Pause"));

    QPushButton::mousePressEvent(eventPress);
}