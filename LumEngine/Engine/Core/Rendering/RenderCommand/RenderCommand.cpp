#include "RenderCommand.h"

bool RenderCommand::operator<(const RenderCommand& other) const
{
	if (isOutline != other.isOutline)
		return !isOutline;

	return material < other.material;
}