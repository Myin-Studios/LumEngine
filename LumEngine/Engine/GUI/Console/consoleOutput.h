#pragma once

#include <QIODevice>
#include <QPlainTextEdit>
#include <QTextCursor>

#include "console.h"

class ConsoleOutput : public QIODevice
{
    Q_OBJECT

public:
    ConsoleOutput(Console* console, QObject* parent = nullptr)
        : QIODevice(parent), m_console(console) {}

    // Override the read and write methods
    qint64 readData(char* data, qint64 maxlen) override { return 0; } // Non usato
    qint64 writeData(const char* data, qint64 len) override
    {
        QString message = QString::fromUtf8(data, len); // Converte il messaggio in QString
        emit newMessage(message); // Invia il messaggio all'output
        return len;
    }

signals:
    void newMessage(const QString& message);

private:
    Console* m_console;
};