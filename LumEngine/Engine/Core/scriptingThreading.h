#pragma once

#include <QThread>
#include "coreclr_delegates.h"

class ScriptRunnerThread : public QThread {

public:
    ScriptRunnerThread(component_entry_point_fn starter, component_entry_point_fn updater, QObject *parent = nullptr)
            : QThread(parent), StartScript(starter), UpdateScript(updater), running(false) {}

    void stop() {
        running = false; // Imposta il flag per fermare il loop
    }

protected:
    void run() override {

        running = true; // Imposta running su true
        
        StartScript(nullptr, 0);

        while (running) {
            // UpdateScript(nullptr, 0);
            QThread::msleep(1000 / 60);
        }
    }

private:
    component_entry_point_fn StartScript; // Puntatore alla funzione StartScript
    component_entry_point_fn UpdateScript; // Puntatore alla funzione UpdateScript
    bool running; // Flag per gestire l'esecuzione
};