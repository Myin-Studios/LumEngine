#pragma once
#include <QFrame>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>

class Vec3Property : public QWidget
{
public:
    Vec3Property(QWidget* parent = nullptr);
    ~Vec3Property();

private:
    QHBoxLayout* layout = nullptr;

    QLabel* xLabel = nullptr;
    QLabel* yLabel = nullptr;
    QLabel* zLabel = nullptr;

    QVBoxLayout* xLayout = nullptr;
    QVBoxLayout* yLayout = nullptr;
    QVBoxLayout* zLayout = nullptr;

    QFrame* xTextFrame = nullptr;
    QFrame* yTextFrame = nullptr;
    QFrame* zTextFrame = nullptr;

    QDoubleSpinBox* xValue = nullptr;
    QDoubleSpinBox* yValue = nullptr;
    QDoubleSpinBox* zValue = nullptr;
};