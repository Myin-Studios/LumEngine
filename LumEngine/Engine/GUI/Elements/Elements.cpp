#include "Elements.h"

UIElements::UIElements() {}

UIElements::~UIElements() {}

void UIElements::addPage(BaseUIElement* elem) {
    this->_elements.push_back(elem);
}

const std::vector<BaseUIElement*>& UIElements::getElements() const {
	return this->_elements;
}

BaseUIElement::BaseUIElement()
{
}

BaseUIElement::BaseUIElement(QWidget* elem) : QWidget(elem)
{
}
