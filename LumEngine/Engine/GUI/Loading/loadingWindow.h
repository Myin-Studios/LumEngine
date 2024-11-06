#pragma once

#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QStyle>
#include <QPainter>
#include <QPainterPath>

#include <QThread>
#include <functional>

class ColouredProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    ColouredProgressBar(QWidget* parent) : QProgressBar(parent)
    {

    }
protected:
    void paintEvent(QPaintEvent* event) override
    {
        Q_UNUSED(event);

        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing); // Abilita l'antialiasing per bordi arrotondati

        int val = value();
        int radius = 3; // Raggio degli angoli arrotondati
        int barHeight = 3; // Altezza della barra

        // Calcola la posizione in base al valore corrente
        int pos = QStyle::sliderPositionFromValue(minimum(), maximum(), val, width() - 50); // Riduci la larghezza per lasciare spazio al testo

        // Crea il rettangolo con angoli arrotondati per la parte di progresso
        QPainterPath progressPath;
        progressPath.addRoundedRect(0, (height() - barHeight) / 2, pos, barHeight, radius, radius);

        // Crea il gradiente lineare per il progresso
        QLinearGradient gradient(0, 0, pos, 0); // Gradiente orizzontale
        gradient.setColorAt(0, QColor("#0088ff")); // Colore iniziale
        gradient.setColorAt(1, QColor("#00baff")); // Colore finale

        // Imposta il gradiente come pennello del pittore
        p.setBrush(QBrush(gradient));
        p.setPen(Qt::NoPen);
        p.drawPath(progressPath);

        // Disegna il testo della progress bar accanto alla barra
        p.setPen(Qt::white);
        p.drawText(width() - 25, (height() + barHeight) / 2, QString::number(val) + "%");
    }
};

class LoadingWindow : public QWidget {
    Q_OBJECT
public:
    LoadingWindow(QWidget *parent = nullptr) : QWidget(parent) {
        setWindowTitle("Loading...");
        QVBoxLayout *layout = new QVBoxLayout(this);

        label = new QLabel("Resources loading...", this);
        layout->addWidget(label);

        progressBar = new ColouredProgressBar(this);
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
    ColouredProgressBar* progressBar;
};