#include "Shaders.h"

ShaderCore::ShaderCore(const char* vertexPath, const char* fragmentPath) : Shader(vertexPath, fragmentPath)
{

}

ShaderCore::~ShaderCore()
{
}

void ShaderCore::use() const
{
	Shader::use();
}

void ShaderCore::setBool(const std::string& name, bool value) const
{
	Shader::setBool(name, value);
}

void ShaderCore::setInt(const std::string& name, int value) const
{
	Shader::setInt(name, value);
}

void ShaderCore::setFloat(const std::string& name, float value) const
{
	Shader::setFloat(name, value);
}

void ShaderCore::setFloatArray(const std::string& name, GLsizei count, const float* value) const
{
	Shader::setFloatArray(name, count, value);
}

void ShaderCore::setVec3(const std::string& name, glm::vec3 v3) const
{
	Shader::setVec3(name, v3);
}

void ShaderCore::setVec3Array(const std::string& name, GLsizei count, const float* value) const
{
	Shader::setVec3Array(name, count, value);
}

void ShaderCore::setMat4x4(const std::string& name, const float* value) const
{
	Shader::setMat4x4(name, value);
}
