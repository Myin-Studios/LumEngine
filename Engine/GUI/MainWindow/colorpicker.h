//
// Created by darkd on 25/06/2024.
//

#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QColorDialog>

class ColorPicker : public QColorDialog
{
    Q_OBJECT

private:
    QColorDialog* _colorPicker;

public:
    ColorPicker(QColorDialog* colPicker) : _colorPicker(colPicker)
    {

    }

public slots:
    void setLastColor(const QColor &color) {
        current = color;
    }

protected:
    void showEvent(QShowEvent *event) override {
        // Chiamato quando il dialogo sta per essere mostrato
        setCurrentColor(current); // Imposta il colore attuale al colore memorizzato

        // Chiama l'implementazione di base
        QColorDialog::showEvent(event);
    }

private:
    QColor current;
};

#endif //COLORPICKER_H
