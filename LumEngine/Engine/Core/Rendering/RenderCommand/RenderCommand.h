#pragma once

#include <glm/glm.hpp>
#include "Engine/GUI/Rendering/Meshes/Mesh.h"
#include "../LumTypes/Entities/Entity.h"

struct RenderCommand
{
	BaseEntity* entity;
	MeshCore* mesh;
	glm::mat4 transform;
	std::shared_ptr<Material> material;
	bool isOutline = false;
	float outlineScale = 1.0005f;

	bool operator<(const RenderCommand& other) const;
};