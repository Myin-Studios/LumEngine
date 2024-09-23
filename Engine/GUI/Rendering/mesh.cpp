#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices)
        : vertices(std::move(vertices)) {
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);

    RendererDebugger::checkOpenGLError("mesh setup");
}

void Mesh::Draw(Shader &shader) const
{
//    unsigned int diffuseNr = 1;
//    unsigned int specularNr = 1;
//    for(unsigned int i = 0; i < textures.size(); i++)
//    {
//        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
//        // retrieve texture number (the N in diffuse_textureN)
//        string number;
//        string name = textures[i].type;
//        if(name == "texture_diffuse")
//            number = std::to_string(diffuseNr++);
//        else if(name == "texture_specular")
//            number = std::to_string(specularNr++);
//
//        shader.setInt(("material." + name + number).c_str(), i);
//        glBindTexture(GL_TEXTURE_2D, textures[i].id);
//    }
//    glActiveTexture(GL_TEXTURE0);

    shader.use();

    // draw mesh

    RendererDebugger::checkOpenGLError("shader usage");

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    RendererDebugger::checkOpenGLError("mesh VAO binding");

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    glBindVertexArray(0);
}