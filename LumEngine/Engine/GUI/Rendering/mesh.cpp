#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices)
        : vertices(std::move(vertices)) {

    transform = new Transform3DCore();
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    std::cout << "Destroying mesh" << std::endl;
}

void Mesh::setupMesh()
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

void Mesh::Draw() const
{
    material->GetShader()->use();

    // glm::mat4 tMat(transform->scale.x(), 0.0f, 0.0f, transform->position->x(),
    //     0.0f, transform->scale.y(), 0.0f, transform->position->y(),
    //     0.0f, 0.0f, transform->scale.z(), transform->position->z(),
    //     0.0f, 0.0f, 0.0f, 1.0f);
    // 
    // material->GetShader()->setMat4x4("model", &tMat[0][0]);

    // draw mesh

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