//
// Created by darkd on 24/06/2024.
//

#ifndef CMAKECACHE_TXT_MAINWINDOW_H
#define CMAKECACHE_TXT_MAINWINDOW_H

#include <QMainWindow>

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QIcon>
#include <QOpenGLWidget>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "3DVisualizer/renderer.h"
#include "QScrollArea"
#include <QColorDialog>
#include "colorpicker.h"
#include <QStyle>
#include <QLabel>
#include <QTextEdit>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QSplitter>
#include <QDoubleSpinBox>
#include <QPropertyAnimation>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

    void init();

private slots:
    void resizeButtonIn()
    {
        QPropertyAnimation anim;
        anim.setTargetObject(playButton);
        anim.setPropertyName("geometry");

        anim.setDuration(500);
        anim.setStartValue(QRect(0, 0, playButton->width(), playButton->height()));
        anim.setEndValue(QRect(0, 0, playButton->width() + 5, playButton->height() + 5));

        anim.start();
    };

    void changeValue()
    {
        renderer->setPosition(xPosEdit->value(), yPosEdit->value(), zPosEdit->value());
    };

    void openColorPicker()
    {
        if (colorPicker == nullptr)
            colorPicker = new ColorPicker(new QColorDialog(nullptr));

        if (renderer != nullptr)
        {
            colorPicker->setLastColor(QColor(255, 0, 0));

            col = colorPicker->getColor();
            renderer->setColors(col);
        }

        if (objectColor != nullptr)
        {
            QString baseColor = QString("QPushButton { background-color: rgb(%1, %2, %3); border: 1px solid black; border-radius: 10px;} }").arg(col.redF() * 255).arg(col.greenF() * 255).arg(col.blueF() * 255);
            QString hoverColor = QString("QPushButton:hover { background-color: rgb(%1, %2, %3); border: 1px solid black; border-radius: 10px;} }").arg(col.redF() * 255).arg(col.greenF() * 255).arg(col.blueF() * 255);
            QString pressedColor = QString("QPushButton:pressed { background-color: rgb(%1, %2, %3); border: 1px solid black; border-radius: 10px;} }").arg(col.redF() * 255).arg(col.greenF() * 255).arg(col.blueF() * 255);

            QString styleSheet = baseColor + " " + hoverColor + " " + pressedColor;

            qDebug() << styleSheet;
            objectColor->setStyleSheet(styleSheet);
        }
    };
protected:
    void resizeEvent(QResizeEvent *event) override
    {
        if (renderer != nullptr)
        {
            renderer->getMainWindowRect(*this);
        }
    }

    void moveEvent(QMoveEvent *event) override
    {
        if (renderer != nullptr)
            renderer->getMainWindowRect(*this);
    }

private:
    Ui::MainWindow *ui;

    QColor col;

    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_as_New;

    QWidget *centralwidget;

    QFrame* propertyPanel;

    QVBoxLayout* propertyLayout;

    QPushButton* objectColor;

    QLabel* colorLabel;
    QLabel* positionLabel;

    QHBoxLayout* positionLayout;
    QFrame* xPos;
    QDoubleSpinBox* xPosEdit;
    QFrame* yPos;
    QDoubleSpinBox* yPosEdit;
    QFrame* zPos;
    QDoubleSpinBox* zPosEdit;

    QGridLayout *mainGridLayout;
    QHBoxLayout *bottomHLayout;
    QHBoxLayout *topHLayout;
    QHBoxLayout *centralHLayout;
    QVBoxLayout *leftVLayout;
    QVBoxLayout *rightVLayout;

    QScrollArea *hierarchy;

    QPushButton *playButton;

    Renderer *renderer;

    ColorPicker* colorPicker;

    QMenuBar *menubar;
    QMenu *menuFile;
};


#endif //CMAKECACHE_TXT_MAINWINDOW_H
