#pragma once

#include "gl/glew.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>
#include <string>
#include <iostream>

#include "shader.h"
#include "rendererDebugger.h"
#include "Engine/Math/Transforms.h"

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
    vector<Vertex> vertices;
    Transform3D* transform = nullptr;

    Mesh() = default;
    ~Mesh();

    Mesh(vector<Vertex> vertices);
    void Draw(Shader &shader) const;
private:
    //  render data
    unsigned int VAO = 0, VBO = 0;

    void setupMesh();
};