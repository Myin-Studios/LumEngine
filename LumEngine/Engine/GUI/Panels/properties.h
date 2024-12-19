#pragma once
#include <QFrame>
#include <QDoubleSpinBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QLineEdit>
#include <QKeyEvent>
#include <QLocale>
#include <QPaintEvent>
#include <QPainterPath>
#include <QStyleOption>

class PropertyGroupHeader : public QFrame
{
public:
    PropertyGroupHeader(const PropertyGroupHeader& other);
    PropertyGroupHeader(const std::string& title = "", QWidget* parent = nullptr);
    ~PropertyGroupHeader();

    std::unique_ptr<QString> getTitle() const;

protected:
    void paintEvent(QPaintEvent* event) override;
private:
    std::unique_ptr<QString> _title;
};

class PropertyGroupContainer : public QFrame
{
public:
    PropertyGroupContainer(QWidget* parent = nullptr);

    QVBoxLayout* getLayout() const;
    void addElement(QWidget* elem, Qt::AlignmentFlag flag = Qt::AlignTop);

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	QVBoxLayout* _mainLayout = nullptr;
};

class PropertyGroup : public QWidget
{
	Q_OBJECT
public:
    PropertyGroup(const std::string& title, QWidget* parent = nullptr);

    QVBoxLayout* getLayout() const;
    void addElement(QWidget* elem, Qt::AlignmentFlag flag = Qt::AlignTop);

private:
	QVBoxLayout* _mainLayout = nullptr;
	PropertyGroupHeader* _header = nullptr;
    PropertyGroupContainer* _content = nullptr;
	QVBoxLayout* _contentLayout = nullptr;
};

enum Coordinate
{
    X,
    Y,
    Z,
    W
};

class NumberOperatorLineEdit : public QLineEdit {
    Q_OBJECT

public:
    NumberOperatorLineEdit(QWidget* parent = nullptr);
    void setCoordinate(Coordinate c = Coordinate::X);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
	void focusOutEvent(QFocusEvent* event) override;

private:
    void evaluateExpression();

    Coordinate _coord = Coordinate::X;

    QString normalStyleX;
    QString normalStyleY;
    QString normalStyleZ;
    QString normalStyleW;
    QString focusStyleX;
    QString focusStyleY;
    QString focusStyleZ;
    QString focusStyleW;
};

class CoordinateFrame : public QFrame
{
public:
    CoordinateFrame(std::string text = "", QWidget* parent = nullptr);
    ~CoordinateFrame();

protected:
	void paintEvent(QPaintEvent* event) override;

private:
	std::string _text;
};


class Vec3Property : public QWidget
{
public:
    Vec3Property(QWidget* parent = nullptr);
    ~Vec3Property();

private:
    QHBoxLayout* _mainLayout = nullptr;

    QLabel* xLabel = nullptr;
    QLabel* yLabel = nullptr;
    QLabel* zLabel = nullptr;

    QVBoxLayout* xLayout = nullptr;
    QVBoxLayout* yLayout = nullptr;
    QVBoxLayout* zLayout = nullptr;

    CoordinateFrame* xTextFrame = nullptr;
    CoordinateFrame* yTextFrame = nullptr;
    CoordinateFrame* zTextFrame = nullptr;

    NumberOperatorLineEdit* xValue = nullptr;
    NumberOperatorLineEdit* yValue = nullptr;
    NumberOperatorLineEdit* zValue = nullptr;
};