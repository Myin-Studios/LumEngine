#pragma once

#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

#include <QThread>
#include <functional>

class LoadingWindow : public QWidget {
    Q_OBJECT
public:
    LoadingWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Loading...");
        QVBoxLayout *layout = new QVBoxLayout(this);

        label = new QLabel("Resources loading...", this);
        layout->addWidget(label);

        progressBar = new QProgressBar(this);
        progressBar->setRange(0, 100);  // Barra di progresso da 0 a 100
        layout->addWidget(progressBar);

        setLayout(layout);
        setFixedSize(300, 100);
    }

    void setProgress(QString text, int value) {
        label->setText("Resources loading... --- " + text);
        progressBar->setValue(value);
    }

    void setThread(QThread* thread)
    {
        loadingThread = thread;
    }

private:
    QThread* loadingThread = nullptr;
    QLabel *label;
    QProgressBar *progressBar;
};