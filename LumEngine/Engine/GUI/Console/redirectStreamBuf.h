#pragma once

#include <QPlainTextEdit>
#include <QString>
#include <iostream>
#include <streambuf>

// Redirect std::cout
class RedirectStreamBuf : public std::streambuf
{
public:
    RedirectStreamBuf(QPlainTextEdit* console)
        : m_console(console) {}

protected:
    int overflow(int c) override
    {
        if (c != EOF)
        {
            // Accumula i caratteri in un buffer
            m_buffer += QString::fromUtf8((const char*)&c, 1);

            // Invia il messaggio se è un carattere di newline
            if (c == '\n')
            {
                emitMessage(m_buffer);
                m_buffer.clear(); // Pulisce il buffer dopo averlo inviato
            }
        }
        return c;
    }

    // Metodo per inviare il messaggio alla console
    void emitMessage(const QString& message)
    {
        if (m_console) {
            QMetaObject::invokeMethod(m_console, "appendMessage", Qt::QueuedConnection, Q_ARG(QString, message));
        }
    }

private:
    QPlainTextEdit* m_console;
    QString m_buffer; // Buffer per accumulare i caratteri
};