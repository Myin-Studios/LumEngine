/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                          LumEngine (C)                                            ///
///                                                                                                   ///
///                                     WHICH IS LICENSED UNDER                                       ///
///                                          MIT License                                              ///
///                                      All Right Reserved.                                          ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///   Copyright (c) 2024 Myin Studios                                                                 ///
///                                                                                                   ///
///   Permission is hereby granted, free of charge,  to any person obtaining a copy of this software  ///
///   and associated documentation files (the "Software"), to deal in the Software without            ///
///   restriction, including without limitation the rights to use, copy, modify, merge, publish,      ///
///   distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the   ///
///   Software is furnished to do so, subject to the following conditions:                            ///
///                                                                                                   ///
///   The above copyright notice and this permission notice shall be included in all copies or        ///
///   substantial portions of the Software.                                                           ///
///                                                                                                   ///
///   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING   ///
///   BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND      ///
///   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,    ///
///   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  ///
///   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.         ///
///                                                                                                   ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include <iostream>

#include "../LumEngine/Engine/GUI/Rendering/Shaders/Shaders.h"
#include "../LumTypes/Rendering/Materials/Materials.h"
#include "../LumEngine/Engine/GUI/Rendering/rendererDebugger.h"
#include "../Vertices/Vertices.h"
#include "../LumTypes/Transformations/Transformations.h"
#include "../LumTypes/Entities/Properties/Property.h"

using namespace std;

struct Texture {
    unsigned int id;
    string type;
};

class MeshCore : public IProperty
{
public:
    vector<Vertex> vertices;
    ~MeshCore()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        std::cout << "Destroying mesh" << std::endl;
    }

    MeshCore(vector<Vertex> vertices = {}) : vertices(std::move(vertices)) {

        setupMesh();
    }

    const vector<Vertex> GetVertices() const { return this->vertices; }

    void Draw() const
    {
        material->GetShader()->use();

        RendererDebugger::checkOpenGLError("shader usage");

        glBindVertexArray(VAO);

        GLint currentProgram;
        glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        if (currentProgram != material->GetShader()->ID) {
            std::cerr << "Shader program not active!" << std::endl;
        }

        RendererDebugger::checkOpenGLError("mesh VAO binding");

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
    }

    void SetMaterial(shared_ptr<Material> m) { this->material = m; }
    shared_ptr<Material> GetMaterial() const { return this->material; }

private:
    //  render data
    unsigned int VAO = 0, VBO = 0;
    shared_ptr<Material> material;

    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        if (!vertices.empty())
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        RendererDebugger::checkOpenGLError("setting up VBO");

        // vertex positions
        glEnableVertexAttribArray(0);

        RendererDebugger::checkOpenGLError("glEnableVertexAttribArray: 0");

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

        RendererDebugger::checkOpenGLError("glVertexAttribPointer: 0");

        // vertex normals
        glEnableVertexAttribArray(1);

        RendererDebugger::checkOpenGLError("glEnableVertexAttribArray: 1");

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        RendererDebugger::checkOpenGLError("glVertexAttribPointer: 1");

        // vertex texture coords
        glEnableVertexAttribArray(2);

        RendererDebugger::checkOpenGLError("glEnableVertexAttribArray: 2");

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        RendererDebugger::checkOpenGLError("glVertexAttribPointer: 2");

        glBindVertexArray(0);

        RendererDebugger::checkOpenGLError("mesh setup");
    }
};