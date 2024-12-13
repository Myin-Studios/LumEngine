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

class PropertyGroup : public QWidget
{
	Q_OBJECT
public:
    PropertyGroup(const std::string& title, QWidget* parent = nullptr);

    QVBoxLayout* getLayout() const;

private:
	QVBoxLayout* _mainLayout = nullptr;
	PropertyGroupHeader* _header = nullptr;
	QFrame* _content = nullptr;
	QVBoxLayout* _contentLayout = nullptr;
};

class NumberOperatorLineEdit : public QLineEdit {
    Q_OBJECT

public:
    NumberOperatorLineEdit(QWidget* parent = nullptr);

    void keyPressEvent(QKeyEvent* event) override;

protected:
	void paintEvent(QPaintEvent* event) override;

private:
    void evaluateExpression();
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