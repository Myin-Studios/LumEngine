#pragma once

#include "gl/glew.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>
#include <iostream>

#include "shader.h"
#include "rendererDebugger.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    string type;
};

class Mesh
{
public:
    vector<Vertex>       vertices;

    Mesh() = default;
    ~Mesh();

    Mesh(vector<Vertex> vertices);
    void Draw(Shader &shader) const;
private:
    //  render data
    unsigned int VAO = 0, VBO = 0;

    void setupMesh();
};