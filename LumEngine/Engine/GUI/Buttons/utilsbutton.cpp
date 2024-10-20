#include "utilsbuttons.h"

PlayButton::PlayButton(QWidget* parent) : BaseButton(parent)
{
    setIcon(QIcon(":/Icons/New/CMEngine_Play"));
}

bool PlayButton::CanPlay() { return this->isPlaying; }

void PlayButton::SetScriptRunner(component_entry_point_fn StartScript, component_entry_point_fn UpdateScript)
{
    this->StartScript = StartScript;
    this->UpdateScript = UpdateScript;
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

    if (!isPlaying) {
        setIcon(QIcon(":/Icons/New/CMEngine_Play"));
        if (scriptRunnerThread) {
            scriptRunnerThread->stop(); // Ferma l'esecuzione
            scriptRunnerThread->wait(); // Aspetta che il thread finisca
            delete scriptRunnerThread; // Libera le risorse
            scriptRunnerThread = nullptr; // Imposta il puntatore a nullptr
        }
    } else {
        setIcon(QIcon(":/Icons/New/CMEngine_Pause"));

        if (CanPlay() && StartScript != nullptr) {
            scriptRunnerThread = new ScriptRunnerThread(StartScript, UpdateScript); // Crea il thread
            scriptRunnerThread->start(); // Avvia il thread
        }
    }

    QPushButton::mousePressEvent(eventPress);
}