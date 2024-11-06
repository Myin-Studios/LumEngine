#pragma once

#include "../LumEngine/Engine/Math/Transforms.h"
#include "../Colors/colors.h"

enum LightType
{
	Directional,
	Point,
	Spot
};

class Light
{
public:
	Light()
	{
		transform = new Transform3DCore();
		color = Color::RGB(1.0f, 1.0f, 1.0f);
	}

	LightType type = LightType::Directional;
	Color::RGB color;

	Transform3DCore* GetTransform() const { return this->transform; }
	void SetTransform(Transform3DCore* t)
	{
		this->transform = t;
	}

	float GetIntensity() const { return this->intensity; }
	void SetIntensity(float val) { this->intensity = val; }
private:
	Transform3DCore* transform;
	float intensity = 1.0f;
};