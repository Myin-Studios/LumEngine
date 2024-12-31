#pragma once

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    Vertex(const Vertex& other) = default;

    Vertex(glm::vec3 pos = glm::vec3(), glm::vec3 nor = glm::vec3(), glm::vec2 tex = glm::vec2()) : Position(pos), Normal(nor), TexCoords(tex) {}
};