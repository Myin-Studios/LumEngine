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

#include "../LumEngine/Engine/GUI/Rendering/Shaders/Shaders.h"
#include "../LumTypes/Rendering/Materials/Materials.h"
#include "../LumEngine/Engine/GUI/Rendering/rendererDebugger.h"
#include "../Vertices/Vertices.h"
#include "../LumTypes/Transformations/Transformations.h"
#include "../LumTypes/Entities/Properties/Property.h"
#include "../LumTypes/LOD/LOD.h"

#include "gl/glew.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;
using namespace LumEngine::Graphics;

struct Texture {
    unsigned int id;
    string type;
};

class MeshCore : public IProperty
{
private:
    // Store LOD data in a more structured way using a map
    // The key is the LOD step, and the value contains both vertices and indices for that LOD level
    struct LODData {
        vector<Vertex> vertices;
        vector<int> indices;
        unsigned int VBO;  // Each LOD level has its own VBO
        unsigned int EBO;  // Element Buffer Object per gli indici

        LODData() : VBO(0), EBO(0) {}

        ~LODData() {
            if (VBO != 0) glDeleteBuffers(1, &VBO);
            if (EBO != 0) glDeleteBuffers(1, &EBO);
        }
    };

    map<int, LODData> lodLevels;  // Maps LOD step to its corresponding data
    unsigned int VAO = 0;
    int currentLOD = 0;
    shared_ptr<Material> material;
    shared_ptr<LODCore> lodSettings;  // Store LOD configuration

public:
    vector<Vertex> vertices;
    vector<int> indices;

    MeshCore(vector<Vertex> vertices = {}, vector<int> indices = {})
        : vertices(std::move(vertices)), indices(std::move(indices))
    {
        // Initialize with base LOD (original mesh)
        LODData baseLevel;
        baseLevel.vertices = this->vertices;
        baseLevel.indices = this->indices;
        lodLevels[0] = baseLevel;

        lodSettings = std::make_shared<LODCore>(0, 100.0f, 0.0f);
        setupMesh();
    }

    const vector<Vertex> GetVertices() const { return this->vertices; }

    void Draw() const
    {
        material->GetShader()->use();

        RendererDebugger::checkOpenGLError("shader usage");

        glBindVertexArray(VAO);

        // Get the current LOD level data
        const auto& currentLevelIt = lodLevels.find(currentLOD);
        if (currentLevelIt == lodLevels.end()) return;

        const auto& currentLevel = currentLevelIt->second;
        glBindBuffer(GL_ARRAY_BUFFER, currentLevel.VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, currentLevel.EBO);

        GLint currentVAO, currentVBO, currentEBO;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &currentVAO);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVBO);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentEBO);

        std::cerr << "Debug Buffers:\n\tVAO: " << currentVAO
            << "\n\tVBO: " << currentVBO
            << "\n\tEBO: " << currentEBO << std::endl;

        if (!currentLevel.indices.empty()) {
            glDrawElements(GL_TRIANGLES, currentLevel.indices.size(),
                GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, currentLevel.vertices.size());
        }

        glBindVertexArray(0);
    }

    void DrawWireframe() const
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

        // Salva lo stato corrente
        GLint previousMode;
        glGetIntegerv(GL_POLYGON_MODE, &previousMode);

        // Imposta il wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glDrawArrays(GL_LINES, 0, vertices.size());

        // Ripristina lo stato precedente
        glPolygonMode(GL_FRONT_AND_BACK, previousMode);

		glBindVertexArray(0);
	}

    void SetMaterial(shared_ptr<Material> m)
    {
        this->material = m;
    }
    shared_ptr<Material> GetMaterial() const { return this->material; }

    void SetLOD(int step)
    {
        // Ensure step is within bounds
        step = max(0, min(step, lodSettings->GetSteps()));

        // Check if this LOD level needs to be generated
        if (step > 0 && lodLevels.find(step) == lodLevels.end()) {
            ProcessLOD(step);
        }

        currentLOD = step;
    }

	void SetLODSettings(shared_ptr<LODCore> settings) { lodSettings = settings; }
	LODCore* GetLODSettings() const { return lodSettings.get(); }
    int GetCurrentLOD() const
    {
        return currentLOD;
    }

private:

    void SetupLODLevel(LODData& level) {
        // Setup VBO for this level
        glGenBuffers(1, &level.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, level.VBO);
        glBufferData(GL_ARRAY_BUFFER,
            level.vertices.size() * sizeof(Vertex),
            level.vertices.data(),
            GL_STATIC_DRAW);

        RendererDebugger::checkOpenGLError("LOD VBO setup");

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0); // Posizione
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); // Normale
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)); // Coordinate UV
        glEnableVertexAttribArray(2);

        // Setup EBO for this level
        glGenBuffers(1, &level.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, level.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            level.indices.size() * sizeof(int),
            level.indices.data(),
            GL_STATIC_DRAW);

        RendererDebugger::checkOpenGLError("LOD EBO setup");
    }

    void ProcessLOD(int step)
    {
        const auto& prevLevel = lodLevels[step - 1];

        LODData newLevel;

        // Soglia basata sul livello di LOD
        // Più alto è step, più aggressiva sarà la decimazione
        float threshold = 3.14159265358979323846 / 4.0f * (1.0f + step * 0.5f);  // Partiamo da 45 gradi

        // We'll process triangles in groups of three vertices
        for (size_t i = 0; i < prevLevel.indices.size(); i += 3)
        {
            // Get the three vertices that form our triangle
            Vertex v1 = prevLevel.vertices[prevLevel.indices[i]];
            Vertex v2 = prevLevel.vertices[prevLevel.indices[i + 1]];
            Vertex v3 = prevLevel.vertices[prevLevel.indices[i + 2]];

            // Calculate the angles between vertex normals
            float angle1 = Vec3Core::Angle(Vec3Core(v1.Normal), Vec3Core(v2.Normal));
            float angle2 = Vec3Core::Angle(Vec3Core(v2.Normal), Vec3Core(v3.Normal));
            float angle3 = Vec3Core::Angle(Vec3Core(v3.Normal), Vec3Core(v1.Normal));

            // Calculate the average angle between normals
            float averageAngle = (angle1 + angle2 + angle3) / 3.0f;

            // If the average angle is less than our threshold, we can merge these vertices
            if (averageAngle < threshold)
            {
                // Create a merged vertex by averaging positions and normals
                Vertex mergedVertex;

                // Average the positions
                mergedVertex.Position = {
                    (v1.Position.x + v2.Position.x + v3.Position.x) / 3.0f,
                    (v1.Position.y + v2.Position.y + v3.Position.y) / 3.0f,
                    (v1.Position.z + v2.Position.z + v3.Position.z) / 3.0f
                };

                // Average and normalize the normals
                Vec3Core mergedNormal(
                    (v1.Normal.x + v2.Normal.x + v3.Normal.x) / 3.0f,
                    (v1.Normal.y + v2.Normal.y + v3.Normal.y) / 3.0f,
                    (v1.Normal.z + v2.Normal.z + v3.Normal.z) / 3.0f
                );
                mergedNormal = mergedNormal.Normalize();
                mergedVertex.Normal = { mergedNormal.x(), mergedNormal.y(), mergedNormal.z() };

                // Average texture coordinates if they exist
                mergedVertex.TexCoords = {
                    (v1.TexCoords.x + v2.TexCoords.x + v3.TexCoords.x) / 3.0f,
                    (v1.TexCoords.y + v2.TexCoords.y + v3.TexCoords.y) / 3.0f
                };

                // Add the merged vertex to our new level
                newLevel.vertices.emplace_back(mergedVertex);

                // Add three indices pointing to the same merged vertex
                unsigned int newIndex = newLevel.vertices.size() - 1;
                newLevel.indices.push_back(newIndex);
                newLevel.indices.push_back(newIndex);
                newLevel.indices.push_back(newIndex);
            }
            else
            {
                // If we're not merging, keep the original vertices and update indices
                unsigned int baseIndex = newLevel.vertices.size();

                // Add the original vertices
                newLevel.vertices.push_back(v1);
                newLevel.vertices.push_back(v2);
                newLevel.vertices.push_back(v3);

                // Add indices for these vertices
                newLevel.indices.push_back(baseIndex);
                newLevel.indices.push_back(baseIndex + 1);
                newLevel.indices.push_back(baseIndex + 2);
            }
        }

        glBindVertexArray(VAO);
        SetupLODLevel(newLevel);
        glBindVertexArray(0);

        // Store the new level in our LOD map
        lodLevels[step] = std::move(newLevel);
	}

    void setupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Set up base level VBO and EBO
        SetupLODLevel(lodLevels[0]);

        glBindVertexArray(0);

        RendererDebugger::checkOpenGLError("mesh setup");
    }
};