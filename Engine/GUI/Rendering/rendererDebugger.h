#pragma once

#include "gl/glew.h"
#include <iostream>

using namespace std;

class RendererDebugger
{
public:
    static void checkOpenGLError(const char* location)
    {
        GLenum error = glGetError();
        while (error != GL_NO_ERROR)
        {
            cerr << "OpenGL error at '" << location << "': " << error << endl;
            error = glGetError();
        }
    }
};