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
#include "Materials/materials.h"
#include "rendererDebugger.h"

#include "Engine/Math/Transforms.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    Vertex(glm::vec3 pos, glm::vec3 nor, glm::vec2 tex) : Position(pos), Normal(nor), TexCoords(tex) {}
};

struct Texture {
    unsigned int id;
    string type;
};

class Mesh
{
public:
    vector<Vertex> vertices;
    Transform3DCore* transform;
    Mesh() = default;
    ~Mesh();

    Mesh(vector<Vertex> vertices);
    void Draw() const;

    void SetMaterial(shared_ptr<Material> m) { this->material = m; }
    shared_ptr<Material> GetMaterial() const { return this->material; }

private:
    //  render data
    unsigned int VAO = 0, VBO = 0;
    shared_ptr<Material> material;

    void setupMesh();
};