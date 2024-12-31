#pragma once

#include "../Entities/Properties/Property.h"

namespace LumEngine::Graphics
{
	class LODCore : public IProperty
	{
	public:
		LODCore(const LODCore& other) = default;
		LODCore(int steps = 0, float maxDist = 10.0f, float minDist = 0.0f);
		virtual ~LODCore() override = default;

		void OnSerialize() override;
		void OnDeserialize() override;

		void SetSteps(int steps);
		int GetSteps() const;

		void SetMaxDistance(float distance);
		float GetMaxDistance() const;

		void SetMinDistance(float distance);
		float GetMinDistance() const;

	private:
		int steps = 0;
		float maxDistance = 0.0f;
		float minDistance = 0.0f;
	};
}