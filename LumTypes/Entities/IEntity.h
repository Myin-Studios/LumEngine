#pragma once

#include "../Physics/ICollisions.h"

class IEntity
{
public:
	virtual bool Intersects(const LumEngine::Physics::IRay& ray, float& distance) = 0;
};