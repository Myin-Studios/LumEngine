#include "LOD.h"

LumEngine::Graphics::LODCore::LODCore(int steps, float maxDist, float minDist) : steps(steps), maxDistance(maxDist), minDistance(minDist) {}

void LumEngine::Graphics::LODCore::OnSerialize()
{
}

void LumEngine::Graphics::LODCore::OnDeserialize()
{
}

void LumEngine::Graphics::LODCore::SetSteps(int steps)
{
	if (steps < 0)
		steps = 0;
	else if (steps > 10)
		steps = 10;

	steps = steps;
}

int LumEngine::Graphics::LODCore::GetSteps() const
{
	return steps;
}

void LumEngine::Graphics::LODCore::SetMaxDistance(float distance)
{
	if (distance < minDistance)
		distance = 0.1f;
	else if (distance > 100.0f)
		distance = 100.0f;

	maxDistance = distance;
}

float LumEngine::Graphics::LODCore::GetMaxDistance() const
{
	return maxDistance;
}

void LumEngine::Graphics::LODCore::SetMinDistance(float distance)
{
	if (distance < 0.0f)
		distance = 0.0f;
	else if (distance > maxDistance)
		distance = 99.0f;

	minDistance = distance;
}

float LumEngine::Graphics::LODCore::GetMinDistance() const
{
	return minDistance;
}
