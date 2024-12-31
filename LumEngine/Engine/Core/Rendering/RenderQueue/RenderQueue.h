#pragma once

#include "../Debugger/AABBDebugger.h"
#include "../RenderCommand/RenderCommand.h"
#include <vector>

class RenderQueue
{
public:
	void push(const RenderCommand& command);
	void pop();
	void sort();
	void clear();
	bool empty() const;
	void process(const glm::mat4& view, const glm::mat4& proj,
		const std::vector<glm::vec3>& lightPositions,
		const std::vector<glm::vec3>& lightColors,
		const std::vector<float>& intensities,
		const glm::vec3& camPos);
	const std::vector<RenderCommand>& getCommands() const;

private:
	std::vector<RenderCommand> _commands;
};