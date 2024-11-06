#pragma once

#include "../LumEngine/Engine/Math/Transforms.h"

class Camera
{
public:
	Camera()
	{
		transform = new Transform3DCore();
	}

	Transform3DCore* GetTransform() const;
	void SetTransform(Transform3DCore* t);
private:
	Transform3DCore* transform;
};