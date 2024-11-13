#pragma once

#include "gl/glew.h"
#include "glm/glm.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use() const
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloatArray(const std::string& name, GLsizei count, const float* value) const;
    void setVec3(const std::string& name, glm::vec3 v3) const;
    void setVec3Array(const std::string& name, GLsizei count, const float* value) const;
    void setMat4x4(const std::string& name, const float* value) const;
};