#include "properties.h"

Vec3Property::Vec3Property(QWidget* parent) : QWidget(parent)
{
    xTextFrame = new QFrame(this);
    yTextFrame = new QFrame(this);
    zTextFrame = new QFrame(this);

    xTextFrame->setStyleSheet(
    "background-color: rgb(255, 0, 0); "
    "border-radius: 2px;");

    yTextFrame->setStyleSheet(
        "background-color: rgb(0, 255, 0); "
        "border-radius: 2px;");

    zTextFrame->setStyleSheet(
        "background-color: rgb(0, 0, 255); "
        "border-radius: 2px;");

    xLabel = new QLabel("X", xTextFrame);
    yLabel = new QLabel("Y", yTextFrame);
    zLabel = new QLabel("Z", zTextFrame);

    QFont font = xLabel->font();
    font.setPointSize(10); // Imposta la dimensione del font
    xLabel->setFont(font);
    yLabel->setFont(font);
    zLabel->setFont(font);

    xLabel->setAlignment(Qt::AlignCenter);
    yLabel->setAlignment(Qt::AlignCenter);
    zLabel->setAlignment(Qt::AlignCenter);

    xLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    yLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    zLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Crea layout per centrare i QLabel nei rispettivi QFrame
    xLayout = new QVBoxLayout(xTextFrame);
    yLayout = new QVBoxLayout(yTextFrame);
    zLayout = new QVBoxLayout(zTextFrame);

    xLayout->addWidget(xLabel, 0, Qt::AlignCenter);
    yLayout->addWidget(yLabel, 0, Qt::AlignCenter);
    zLayout->addWidget(zLabel, 0, Qt::AlignCenter);

    xLayout->setContentsMargins(0, 0, 0, 0);
    yLayout->setContentsMargins(0, 0, 0, 0);
    zLayout->setContentsMargins(0, 0, 0, 0);

    xTextFrame->setLayout(xLayout);
    yTextFrame->setLayout(yLayout);
    zTextFrame->setLayout(zLayout);

    xValue = new QDoubleSpinBox(this);
    yValue = new QDoubleSpinBox(this);
    zValue = new QDoubleSpinBox(this);

    xTextFrame->setFixedSize(20, 20);
    yTextFrame->setFixedSize(20, 20);
    zTextFrame->setFixedSize(20, 20);

    xValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    yValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    zValue->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    layout = new QHBoxLayout(parent);

    layout->addWidget(xTextFrame);
    layout->addWidget(xValue);
    layout->addWidget(yTextFrame);
    layout->addWidget(yValue);
    layout->addWidget(zTextFrame);
    layout->addWidget(zValue);

    xTextFrame->show();
    yTextFrame->show();
    zTextFrame->show();

    xValue->show();
    yValue->show();
    zValue->show();

    setLayout(layout);
}

Vec3Property::~Vec3Property()
{
    delete xValue;
    delete yValue;
    delete zValue;
    delete xLabel;
    delete yLabel;
    delete zLabel;
    delete xTextFrame;
    delete yTextFrame;
    delete zTextFrame;
    delete layout;
}