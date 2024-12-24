#pragma once

#include <iostream>

class IGUIBuilder
{
public:
	virtual ~IGUIBuilder() = default;

	virtual void AddElement(const std::string& title, const std::string& propTitle) = 0;
	virtual void RemoveAllElements() = 0;
};