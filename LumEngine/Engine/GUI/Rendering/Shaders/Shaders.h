#pragma once

#include "../LumTypes/Rendering/Shaders/Shader.h"

class ShaderCore : public Shader
{
public:
	ShaderCore(const char* vertexPath, const char* fragmentPath);
	~ShaderCore();

    void use() const;
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloatArray(const std::string& name, GLsizei count, const float* value) const;
    void setVec3(const std::string& name, glm::vec3 v3) const;
    void setVec3Array(const std::string& name, GLsizei count, const float* value) const;
    void setMat4x4(const std::string& name, const float* value) const;
};