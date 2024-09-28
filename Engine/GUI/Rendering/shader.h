#pragma once

#include "gl/glew.h"
#include "glm/glm.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    // use/activate the shader
    void use() const;
    // utility uniform functions
    void setBool(const string &name, bool value) const;
    void setInt(const string &name, int value) const;
    void setFloat(const string &name, float value) const;
    void setMat4x4(const string &name, const float* value) const;
};