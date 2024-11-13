#pragma once

#include "../LumTypes/Rendering/Shaders/Shader.h"

class ShaderCore : public Shader
{
public:
	ShaderCore(const char* vertexPath, const char* fragmentPath);
	~ShaderCore();

    using Shader::use;

    using Shader::setBool;
    using Shader::setInt;
    using Shader::setFloat;
    using Shader::setFloatArray;
    using Shader::setVec3;
    using Shader::setVec3Array;
    using Shader::setMat4x4;
};