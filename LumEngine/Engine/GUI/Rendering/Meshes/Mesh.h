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
#include "Engine/Core/Caching/BufferCache.h"

#include "gl/glew.h"
#include <vector>
#include <string>
#include <iostream>
#include <map>
#include <queue>
#include <set>

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
        vector<unsigned int> indices;
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

	std::string _path;

public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;

    MeshCore(vector<Vertex> vertices = {}, vector<unsigned int> indices = {}, const std::string& path = "")
        : vertices(std::move(vertices)), indices(std::move(indices))
    {
        // Initialize with base LOD (original mesh)
        LODData baseLevel;
        baseLevel.vertices = this->vertices;
        baseLevel.indices = this->indices;
        lodLevels[0] = baseLevel;

        lodSettings = std::make_shared<LODCore>(0, 100.0f, 0.0f);

		_path = path;

        setupMesh();
    }

	~MeshCore()
	{
		if (VAO != 0) glDeleteVertexArrays(1, &VAO);

		auto it = find_if(lodLevels.begin(), lodLevels.end(), [](const pair<int, LODData>& level) {
			return level.second.VBO != 0;
			});

        if (it != lodLevels.end())
        {
            if (!it->second.vertices.empty())
            {
                it->second.vertices.clear();
            }
            if (!it->second.indices.empty())
			{
				it->second.indices.clear();
			}

			if (it->second.EBO != 0) glDeleteBuffers(1, &it->second.EBO);
            glDeleteBuffers(1, &it->second.VBO);
        }

        lodLevels.clear();
	}

	void SetPath(const std::string& path) { _path = path; }

    const vector<Vertex> GetVertices() const { return this->vertices; }

    void Draw() const
    {
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

        auto buffers = GLBufferCache::GetInstance()->GetOrCreateBuffers(
            _path,
            lodLevels[step].vertices,
            lodLevels[step].indices,
            step
        );

        // Usa i buffer del livello LOD appropriato
        VAO = buffers->VAO;
		lodLevels[step].VBO = buffers->VBO;
		lodLevels[step].EBO = buffers->EBO;
    }

	void SetLODSettings(shared_ptr<LODCore> settings) { lodSettings = settings; }
	LODCore* GetLODSettings() const { return lodSettings.get(); }
    int GetCurrentLOD() const
    {
        return currentLOD;
    }

private:

    void SetupLODLevel(LODData& level) {
        // Generate and set up VBO
        glGenBuffers(1, &level.VBO);
        glBindBuffer(GL_ARRAY_BUFFER, level.VBO);
        glBufferData(GL_ARRAY_BUFFER,
            level.vertices.size() * sizeof(Vertex),
            level.vertices.data(),
            GL_STATIC_DRAW);

        // Generate and set up EBO
        glGenBuffers(1, &level.EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, level.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            level.indices.size() * sizeof(int),
            level.indices.data(),
            GL_STATIC_DRAW);

        RendererDebugger::checkOpenGLError("LOD buffer setup");
    }

    void ProcessLOD(int step) {
        const auto& prevLevel = lodLevels[step - 1];

        // Fase 1: Vertex Welding
        float weldThreshold = 0.0001f;
        std::vector<int> vertexRemap(prevLevel.vertices.size(), -1);
        std::vector<Vertex> unifiedVertices;

        for (size_t i = 0; i < prevLevel.vertices.size(); i++) {
            if (vertexRemap[i] != -1) continue;

            const Vec3Core v1Pos(prevLevel.vertices[i].Position.x,
                prevLevel.vertices[i].Position.y,
                prevLevel.vertices[i].Position.z);

            vertexRemap[i] = unifiedVertices.size();
            unifiedVertices.push_back(prevLevel.vertices[i]);

            for (size_t j = i + 1; j < prevLevel.vertices.size(); j++) {
                if (vertexRemap[j] != -1) continue;

                const Vec3Core v2Pos(prevLevel.vertices[j].Position.x,
                    prevLevel.vertices[j].Position.y,
                    prevLevel.vertices[j].Position.z);

                Vec3Core diff = v2Pos - v1Pos;
                if (diff.Length() < weldThreshold) {
                    vertexRemap[j] = vertexRemap[i];
                    std::cout << "Welding vertex " << j << " to " << i << std::endl;
                }
            }
        }

        // Crea gli indici unificati
        std::vector<unsigned int> unifiedIndices;
        for (size_t i = 0; i < prevLevel.indices.size(); i++) {
            unifiedIndices.push_back(vertexRemap[prevLevel.indices[i]]);
        }

        std::cout << "Vertex welding statistics:" << std::endl;
        std::cout << "Original vertices: " << prevLevel.vertices.size() << std::endl;
        std::cout << "Unified vertices: " << unifiedVertices.size() << std::endl;

        // Crea la mesh unificata
        LODData unifiedMesh;
        unifiedMesh.vertices = std::move(unifiedVertices);
        unifiedMesh.indices = std::move(unifiedIndices);

        // Fase 2: Edge Collapse
        const auto& meshToProcess = unifiedMesh;
        LODData newLevel = meshToProcess;

        // Costruisci la mappa vertice->facce usando set per evitare duplicati
        std::vector<std::set<unsigned int>> vertexToFacesSet(meshToProcess.vertices.size());
        for (size_t i = 0; i < meshToProcess.indices.size(); i += 3) {
            unsigned int faceIndex = i / 3;
            unsigned int i1 = meshToProcess.indices[i];
            unsigned int i2 = meshToProcess.indices[i + 1];
            unsigned int i3 = meshToProcess.indices[i + 2];

            vertexToFacesSet[i1].insert(faceIndex);
            vertexToFacesSet[i2].insert(faceIndex);
            vertexToFacesSet[i3].insert(faceIndex);
        }

        // Converti i set in vector
        std::vector<std::vector<unsigned int>> vertexToFaces(meshToProcess.vertices.size());
        for (size_t i = 0; i < vertexToFacesSet.size(); i++) {
            vertexToFaces[i] = std::vector<unsigned int>(vertexToFacesSet[i].begin(), vertexToFacesSet[i].end());
        }

        // Costruisci la coda degli edge da collassare
        std::priority_queue<Edge> edgeQueue;
        std::set<std::pair<unsigned int, unsigned int>> processedEdges;

        for (size_t i = 0; i < meshToProcess.indices.size(); i += 3) {
            for (int j = 0; j < 3; j++) {
                unsigned int v1 = meshToProcess.indices[i + j];
                unsigned int v2 = meshToProcess.indices[i + ((j + 1) % 3)];

                if (v1 > v2) std::swap(v1, v2);

                if (processedEdges.insert({ v1, v2 }).second) {
                    const Vec3Core v1Pos(meshToProcess.vertices[v1].Position.x,
                        meshToProcess.vertices[v1].Position.y,
                        meshToProcess.vertices[v1].Position.z);
                    const Vec3Core v2Pos(meshToProcess.vertices[v2].Position.x,
                        meshToProcess.vertices[v2].Position.y,
                        meshToProcess.vertices[v2].Position.z);
                    Vec3Core collapsePoint = (v1Pos + v2Pos) * 0.5f;

                    float cost = (v2Pos - v1Pos).Length();
                    edgeQueue.emplace(v1, v2, cost, collapsePoint);
                }
            }
        }

        // Esegui i collapse
        int targetTriangles = meshToProcess.indices.size() / 6;  // Riduci di metà
        int currentTriangles = meshToProcess.indices.size() / 3;
        std::vector<bool> vertexRemoved(meshToProcess.vertices.size(), false);
        std::vector<bool> triangleRemoved(meshToProcess.indices.size() / 3, false);

        while (!edgeQueue.empty() && currentTriangles > targetTriangles) {
            Edge edge = edgeQueue.top();
            edgeQueue.pop();

            if (vertexRemoved[edge.v1] || vertexRemoved[edge.v2]) continue;

            EdgeTopology topo = EdgeTopology::analyzeEdgeTopology(
                edge.v1, edge.v2, vertexToFaces, meshToProcess.indices);

            if (!EdgeTopology::isValidCollapse(topo, edge.collapsePoint,
                meshToProcess.vertices, meshToProcess.indices)) {
                continue;
            }

            // Rimuovi i triangoli che verranno eliminati
            int removedTris = 0;
            for (unsigned int triIndex : topo.adjacentTriangles) {
                if (!triangleRemoved[triIndex]) {
                    triangleRemoved[triIndex] = true;
                    removedTris++;
                }
            }

            currentTriangles -= removedTris;

            // Aggiorna il vertice v1 con il punto di collasso
            newLevel.vertices[edge.v1].Position = {
                edge.collapsePoint.x(),
                edge.collapsePoint.y(),
                edge.collapsePoint.z()
            };

            // Interpola gli attributi
            const Vertex& v1 = meshToProcess.vertices[edge.v1];
            const Vertex& v2 = meshToProcess.vertices[edge.v2];

            Vec3Core normal1(v1.Normal.x, v1.Normal.y, v1.Normal.z);
            Vec3Core normal2(v2.Normal.x, v2.Normal.y, v2.Normal.z);
            Vec3Core newNormal = (normal1 + normal2).Normalize();

            newLevel.vertices[edge.v1].Normal = {
                newNormal.x(),
                newNormal.y(),
                newNormal.z()
            };

            newLevel.vertices[edge.v1].TexCoords = {
                (v1.TexCoords.x + v2.TexCoords.x) * 0.5f,
                (v1.TexCoords.y + v2.TexCoords.y) * 0.5f
            };

            vertexRemoved[edge.v2] = true;

            // Aggiorna i riferimenti a v2 con v1
            for (size_t i = 0; i < newLevel.indices.size(); i++) {
                if (newLevel.indices[i] == edge.v2) {
                    newLevel.indices[i] = edge.v1;
                }
            }
        }

        // Compatta la mesh finale
        LODData compactedLevel;
        std::vector<int> finalRemap(meshToProcess.vertices.size(), -1);

        for (size_t i = 0; i < meshToProcess.vertices.size(); i++) {
            if (!vertexRemoved[i]) {
                finalRemap[i] = compactedLevel.vertices.size();
                compactedLevel.vertices.push_back(newLevel.vertices[i]);
            }
        }

        for (size_t i = 0; i < meshToProcess.indices.size(); i += 3) {
            if (!triangleRemoved[i / 3]) {
                for (int j = 0; j < 3; j++) {
                    unsigned int oldIndex = newLevel.indices[i + j];
                    compactedLevel.indices.push_back(finalRemap[oldIndex]);
                }
            }
        }

        // Output statistics
        std::cout << "LOD Level " << step << " statistics:" << std::endl;
        std::cout << "Original triangles: " << prevLevel.indices.size() / 3 << std::endl;
        std::cout << "Final triangles: " << compactedLevel.indices.size() / 3 << std::endl;
        std::cout << "Reduction: " <<
            (1.0f - (float)(compactedLevel.indices.size() / 3) / (prevLevel.indices.size() / 3)) * 100.0f << "%" << std::endl;

        // Set up the new level's buffers
        glBindVertexArray(VAO);
        SetupLODLevel(compactedLevel);
        glBindVertexArray(0);

        // Store the new level
        lodLevels[step] = std::move(compactedLevel);
    }

    void setupMesh() {
		auto buffers = GLBufferCache::GetInstance()->GetOrCreateBuffers(_path, vertices, indices);

        VAO = buffers->VAO;
		lodLevels[0].VBO = buffers->VBO;
		lodLevels[0].EBO = buffers->EBO;

        // Create and bind VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // Set up base level buffers
        SetupLODLevel(lodLevels[0]);

        glBindVertexArray(0);
        RendererDebugger::checkOpenGLError("mesh setup");
    }
};