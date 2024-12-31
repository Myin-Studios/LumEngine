#pragma once

#include <QLineEdit>
#include <QPaintEvent>
#include <QObject>

#include <iostream>

class TextField : public QLineEdit
{
	Q_OBJECT
public:
	TextField(QWidget* parent = nullptr);

private:
	void paintEvent(QPaintEvent* e) override;

	std::string focusedStyle;
	std::string unfocusedStyle;
public:
};