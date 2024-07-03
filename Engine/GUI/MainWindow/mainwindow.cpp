//
// Created by darkd on 24/06/2024.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
#include <cmath>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::init()
{
    if (this->objectName().isEmpty())
        this->setObjectName("MainWindow");
    this->resize(702, 400);
    this->setStyleSheet(QString::fromUtf8("background-color: rgb(35, 35, 35);\n"
                                                "color: rgb(255, 255, 255);"));

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);

    QSurfaceFormat::setDefaultFormat(format);

    centralwidget = new QWidget(this);

    this->setCentralWidget(centralwidget);

    mainGridLayout = new QGridLayout(centralwidget);
    topHLayout = new QHBoxLayout(nullptr);
    centralHLayout = new QHBoxLayout(nullptr);
    leftVLayout = new QVBoxLayout(nullptr);

    hierarchy = new QScrollArea(nullptr);
    hierarchy->setFixedHeight(100);

    playButton = new QPushButton(nullptr);
    playButton->setFixedHeight(30);
    playButton->setFixedWidth(30);
    playButton->setIcon(QIcon(":/Assets/Buttons/PlayButton.png"));
    playButton->setStyleSheet("QPushButton {"
                              "background-color: rgb(40, 40, 40);"
                              "border: 1px solid;"
                              "border-radius: 7px;"
                              "border-color: rgb(40, 40, 40);}"
                              "QPushButton:hover {"
                              "background-color: rgb(45, 45, 45);}"
                              "QPushButton:pressed {"
                              "background-color: rgb(30, 30, 30);}");

    renderer = new Renderer(nullptr);
    renderer->resize(this->width(), this->height());
    renderer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    renderer->setAcceptDrops(true);
    renderer->getMainWindowRect(*this);

    propertyPanel = new QFrame(nullptr);
    propertyPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    propertyPanel->setGeometry(0, 0, this->width() / 4, propertyPanel->height());

    QSplitter* hSplitter = new QSplitter(Qt::Horizontal, nullptr);
    hSplitter->addWidget(renderer);
    hSplitter->addWidget(propertyPanel);

    propertyLayout = new QVBoxLayout(propertyPanel);

    colorLabel = new QLabel("Color", nullptr);

    objectColor = new QPushButton(nullptr);
    objectColor->setFixedHeight(20);
    objectColor->setStyleSheet("QPushButton {"
                               "background-color: black;"
                               "border: 1px solid black;"
                               "border-radius: 10px;}"
                               "QPushButton:hover {"
                               "background-color: black;}"
                               "QPushButton:pressed {"
                               "background-color: black;}");
    objectColor->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    positionLabel = new QLabel("Position", nullptr);

    positionLayout = new QHBoxLayout(nullptr);

    xPos = new QFrame(nullptr);
    xPos->setFixedWidth(propertyPanel->width() / 6 - 5);
    xPos->setFixedHeight(propertyPanel->width() / 6 - 5);
    xPos->setStyleSheet("QFrame { background-color: rgb(255, 0, 0);  border-radius: 7px }");
    xPosEdit = new QDoubleSpinBox(nullptr);
    xPosEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    xPosEdit->setFixedHeight(propertyPanel->width() / 6);

    auto* xLabel = new QLabel("x", nullptr);
    xLabel->setStyleSheet("font-size: 15px;");
    auto* xLabelLayout = new QHBoxLayout(nullptr);
    xLabelLayout->addWidget(xLabel);
    xLabelLayout->setAlignment(Qt::AlignCenter);
    xLabelLayout->setContentsMargins(0, 0, 0, 0);

    yPos = new QFrame(nullptr);
    yPos->setFixedWidth(propertyPanel->width() / 6 - 5);
    yPos->setFixedHeight(propertyPanel->width() / 6 - 5);
    yPos->setStyleSheet("QFrame { background-color: rgb(0, 255, 0);  border-radius: 7px }");
    yPosEdit = new QDoubleSpinBox(nullptr);
    yPosEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    yPosEdit->setFixedHeight(propertyPanel->width() / 6);

    auto* yLabel = new QLabel("y", nullptr);
    yLabel->setStyleSheet("font-size: 15px;");
    auto* yLabelLayout = new QHBoxLayout(nullptr);
    yLabelLayout->addWidget(yLabel);
    yLabelLayout->setAlignment(Qt::AlignCenter);
    yLabelLayout->setContentsMargins(0, 0, 0, 0);

    zPos = new QFrame(nullptr);
    zPos->setFixedWidth(propertyPanel->width() / 6 - 5);
    zPos->setFixedHeight(propertyPanel->width() / 6 - 5);
    zPos->setStyleSheet("QFrame { background-color: rgb(0, 0, 255); border-radius: 7px }");
    zPosEdit = new QDoubleSpinBox(nullptr);
    zPosEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    zPosEdit->setFixedHeight(propertyPanel->width() / 6);

    auto* zLabel = new QLabel("z", nullptr);
    zLabel->setStyleSheet("font-size: 15px;");
    auto* zLabelLayout = new QHBoxLayout(nullptr);
    zLabelLayout->addWidget(zLabel);
    zLabelLayout->setAlignment(Qt::AlignCenter);
    zLabelLayout->setContentsMargins(0, 0, 0, 0);

    xPos->setLayout(xLabelLayout);
    yPos->setLayout(yLabelLayout);
    zPos->setLayout(zLabelLayout);

    xPosEdit->setMinimum(INFINITY * -1);
    xPosEdit->setMaximum(INFINITY);
    yPosEdit->setMinimum(INFINITY * -1);
    yPosEdit->setMaximum(INFINITY);
    zPosEdit->setMinimum(INFINITY * -1);
    zPosEdit->setMaximum(INFINITY);

    xPosEdit->setSingleStep(0.1);
    yPosEdit->setSingleStep(0.1);
    zPosEdit->setSingleStep(0.1);

    connect(xPosEdit, &QDoubleSpinBox::valueChanged, this, &MainWindow::changeValue);
    connect(yPosEdit, &QDoubleSpinBox::valueChanged, this, &MainWindow::changeValue);
    connect(zPosEdit, &QDoubleSpinBox::valueChanged, this, &MainWindow::changeValue);

    positionLayout->addWidget(xPos);
    positionLayout->addWidget(xPosEdit);
    positionLayout->addWidget(yPos);
    positionLayout->addWidget(yPosEdit);
    positionLayout->addWidget(zPos);
    positionLayout->addWidget(zPosEdit);

    propertyLayout->setAlignment(Qt::AlignTop);
    positionLayout->setAlignment(Qt::AlignTop);

    propertyLayout->addWidget(colorLabel);
    propertyLayout->addWidget(objectColor);
    propertyLayout->addWidget(positionLabel);
    propertyLayout->addLayout(positionLayout);

    topHLayout->addWidget(playButton);
    centralHLayout->addLayout(leftVLayout);
//    centralHLayout->addWidget(renderer);
    centralHLayout->addWidget(hSplitter);

    mainGridLayout->addLayout(topHLayout, 0, 0, 1, 3);
    mainGridLayout->addWidget(hSplitter, 1, 0, 1, 3);

    colorPicker = new ColorPicker(new QColorDialog(nullptr));

    QPushButton::connect(objectColor, SIGNAL(clicked()), this, SLOT(openColorPicker()));

    renderer->show();
    playButton->show();
}
