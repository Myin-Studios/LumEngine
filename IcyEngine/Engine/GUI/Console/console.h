#pragma once

#include <QPlainTextEdit>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollBar>

class Console : public QPlainTextEdit
{
    Q_OBJECT

public:
    explicit Console(QWidget* parent = nullptr)
        : QPlainTextEdit(parent)
    {
        setReadOnly(true); // Imposta la console in modalità sola lettura
    }

public slots:
    void appendMessage(const QString& message)
    {
        appendPlainText(message); // Aggiunge il testo alla fine della console
        verticalScrollBar()->setValue(verticalScrollBar()->maximum()); // Scorre automaticamente alla fine
    }
};