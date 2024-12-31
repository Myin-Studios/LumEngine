#pragma once

#include "../LumTypes/Entities/Entity.h"
#include <iostream>

class IGUIBuilder
{
public:
	virtual ~IGUIBuilder() = default;

	virtual void SetEntity(BaseEntity* e) = 0;
	virtual BaseEntity* GetEntity() = 0;

	virtual void AddElement(const std::string& title, const std::string& propTitle) = 0;
	virtual void RemoveAllElements() = 0;
};