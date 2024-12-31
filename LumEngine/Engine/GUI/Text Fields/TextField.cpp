#include "TextField.h"

TextField::TextField(QWidget* parent) : QLineEdit(parent)
{
	focusedStyle =
		"QLineEdit {"
		"background-color: rgb(25, 25, 25);"
		"border: 2 solid rgb(0, 127, 255);"
		"border-radius: 7px;"
		"color: rgb(255, 255, 255);"
		"}";

	unfocusedStyle =
		"QLineEdit {"
		"background-color: rgb(25, 25, 25);"
		"border: 2 solid rgb(30, 30, 30);"
		"border-radius: 7px;"
		"color: rgb(255, 255, 255);"
		"}";

	this->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
	this->setMinimumSize(0, 30);
}

void TextField::paintEvent(QPaintEvent* e)
{
	QLineEdit::paintEvent(e);

	if (this->hasFocus())
	{
		this->setStyleSheet(QString::fromStdString(focusedStyle));
	}
	else
	{
		this->setStyleSheet(QString::fromStdString(unfocusedStyle));
	}
}
