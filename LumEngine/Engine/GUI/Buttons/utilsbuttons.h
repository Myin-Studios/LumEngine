#pragma once

#include "basebutton.h"
#include "coreclr_delegates.h"
#include "nethost.h"
#include "hostfxr.h"

#include <QThread>

#include "Engine/Core/scriptingThreading.h"

class PlayButton : public BaseButton
{
public:
    PlayButton(QWidget* parent = nullptr);
    bool CanPlay();
    void SetScriptRunner(component_entry_point_fn StartScript, component_entry_point_fn UpdateScript);

protected:
    virtual void enterEvent(QEnterEvent* event);
    virtual void leaveEvent(QEvent* event);
    virtual void mousePressEvent(QMouseEvent *eventPress);

private:
    ScriptRunnerThread* scriptRunnerThread;
    QThread* scriptThread = nullptr;
    component_entry_point_fn StartScript = nullptr;
    component_entry_point_fn UpdateScript = nullptr;
    bool isPlaying = false;
};