#pragma once

#include <QWidget>
#include <QObject>
#include <QVector>

#include <iostream>
#include <vector>
#include <memory>

class BaseUIElement : public QWidget
{
public:
	BaseUIElement();
	BaseUIElement(QWidget* elem);
    ~BaseUIElement() = default;

private:

};

class UIElements
{
public:
    UIElements();
    ~UIElements();

    void addPage(BaseUIElement* elem);
    const std::vector<BaseUIElement*>& getElements() const;

private:
    std::vector<BaseUIElement*> _elements;
};