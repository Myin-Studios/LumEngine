#pragma once

typedef int (*component_entry_point_fn)(void* data, int size);

class IScriptRunner {
public:
	virtual ~IScriptRunner() = default;
	virtual void stop() = 0;
	virtual bool IsRunning() const = 0;
	virtual void setScripts(component_entry_point_fn start, component_entry_point_fn update) = 0;
};