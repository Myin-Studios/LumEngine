#pragma once

#include "../LumTypes/Mathematics/Math.h"

#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <vector>
#include <set>

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;

    Vertex(const Vertex& other) = default;

    Vertex(glm::vec3 pos = glm::vec3(), glm::vec3 nor = glm::vec3(), glm::vec2 tex = glm::vec2()) : Position(pos), Normal(nor), TexCoords(tex) {}
};

struct SharedEdge {
    unsigned int v1, v2;
    std::vector<unsigned int> triangleIndices;
};

struct VertexInfo {
    std::set<unsigned int> adjacentVertices;
    std::set<unsigned int> triangles;  // Indici dei triangoli che usano questo vertice
};

struct Quadric {
    // Invece di memorizzare una matrice 4x4, memorizziamo solo i coefficienti del piano
    std::vector<Vec3Core> planes;  // Normali dei piani
    std::vector<float> distances;  // Distanze dall'origine

    Quadric() {}

    void add(const Vec3Core& planeNormal, float planeDistance) {
        planes.push_back(planeNormal);
        distances.push_back(planeDistance);
    }

    void add(const Quadric& other) {
        planes.insert(planes.end(), other.planes.begin(), other.planes.end());
        distances.insert(distances.end(), other.distances.begin(), other.distances.end());
    }

    float evaluate(const Vec3Core& point) const {
        float error = 0.0f;
        for (size_t i = 0; i < planes.size(); i++) {
            float distance = planes[i] * point + distances[i];
            error += distance * distance;
        }
        return error;
    }

    // Trova il punto che minimizza l'errore
    bool findOptimalPosition(Vec3Core& result, const Vec3Core& v1Pos, const Vec3Core& v2Pos) const {
        // Calcoliamo il punto lungo l'edge che minimizza l'errore
        float minError = std::numeric_limits<float>::max();
        const int STEPS = 10;  // Numero di punti da provare lungo l'edge

        for (int i = 0; i <= STEPS; i++) {
            float t = static_cast<float>(i) / STEPS;
            Vec3Core testPoint = v1Pos * (1.0f - t) + v2Pos * t;
            float error = evaluate(testPoint);

            if (error < minError) {
                minError = error;
                result = testPoint;
            }
        }

        return true;
    }

    static void ComputeQuadricForVertex(const Vertex& vertex, const Vec3Core& faceNormal, Quadric& q) {
        // Calcola d per l'equazione del piano ax + by + cz + d = 0
        float d = -(faceNormal.x() * vertex.Position.x +
            faceNormal.y() * vertex.Position.y +
            faceNormal.z() * vertex.Position.z);

        q.add(faceNormal, d);
    }
};

struct Edge {
    unsigned int v1, v2;
    float cost;
    Vec3Core collapsePoint;

    Edge(unsigned int a, unsigned int b, float c, const Vec3Core& p)
        : v1(std::min(a, b)), v2(std::max(a, b)), cost(c), collapsePoint(p) {}

    bool operator<(const Edge& other) const {
        return cost > other.cost;  // Per la priority_queue (minimo costo in cima)
    }
};

struct EdgeTopology {
    std::vector<unsigned int> sharedVertices;
    std::vector<unsigned int> adjacentTriangles;
    bool isBoundary;

    static EdgeTopology analyzeEdgeTopology(unsigned int v1, unsigned int v2,
        const std::vector<std::vector<unsigned int>>& vertexToFaces,
        const std::vector<unsigned int>& indices) {
        EdgeTopology topo;
        std::set<unsigned int> sharedVertices;
        std::set<unsigned int> adjacentTris;

        std::cout << "\nAnalyzing topology for edge " << v1 << "-" << v2 << std::endl;
        std::cout << "v1 has " << vertexToFaces[v1].size() << " faces" << std::endl;
        std::cout << "v2 has " << vertexToFaces[v2].size() << " faces" << std::endl;

        for (unsigned int faceIdx : vertexToFaces[v1]) {
            unsigned int i1 = indices[faceIdx * 3];
            unsigned int i2 = indices[faceIdx * 3 + 1];
            unsigned int i3 = indices[faceIdx * 3 + 2];

            if (i1 == v2 || i2 == v2 || i3 == v2) {
                adjacentTris.insert(faceIdx);
                if (i1 != v1 && i1 != v2) sharedVertices.insert(i1);
                if (i2 != v1 && i2 != v2) sharedVertices.insert(i2);
                if (i3 != v1 && i3 != v2) sharedVertices.insert(i3);
            }
        }

        for (unsigned int faceIdx : vertexToFaces[v2]) {
            unsigned int i1 = indices[faceIdx * 3];
            unsigned int i2 = indices[faceIdx * 3 + 1];
            unsigned int i3 = indices[faceIdx * 3 + 2];

            if (i1 == v1 || i2 == v1 || i3 == v1) {
                adjacentTris.insert(faceIdx);
                if (i1 != v1 && i1 != v2) sharedVertices.insert(i1);
                if (i2 != v1 && i2 != v2) sharedVertices.insert(i2);
                if (i3 != v1 && i3 != v2) sharedVertices.insert(i3);
            }
        }

        topo.sharedVertices = std::vector<unsigned int>(sharedVertices.begin(), sharedVertices.end());
        topo.adjacentTriangles = std::vector<unsigned int>(adjacentTris.begin(), adjacentTris.end());
        topo.isBoundary = adjacentTris.size() <= 2;

        std::cout << "Found " << topo.adjacentTriangles.size() << " adjacent triangles" << std::endl;
        std::cout << "Found " << topo.sharedVertices.size() << " shared vertices" << std::endl;
        std::cout << "Edge is " << (topo.isBoundary ? "boundary" : "internal") << std::endl;

        return topo;
    }

    static bool isValidCollapse(const EdgeTopology& topo, const Vec3Core& newPos,
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices) {
        std::cout << "\nValidating collapse with " << topo.adjacentTriangles.size() << " adjacent triangles" << std::endl;
        std::cout << "New position: (" << newPos.x() << ", " << newPos.y() << ", " << newPos.z() << ")" << std::endl;

        for (unsigned int triIdx : topo.adjacentTriangles) {
            const Vec3Core v1(vertices[indices[triIdx * 3]].Position.x,
                vertices[indices[triIdx * 3]].Position.y,
                vertices[indices[triIdx * 3]].Position.z);
            const Vec3Core v2(vertices[indices[triIdx * 3 + 1]].Position.x,
                vertices[indices[triIdx * 3 + 1]].Position.y,
                vertices[indices[triIdx * 3 + 1]].Position.z);
            const Vec3Core v3(vertices[indices[triIdx * 3 + 2]].Position.x,
                vertices[indices[triIdx * 3 + 2]].Position.y,
                vertices[indices[triIdx * 3 + 2]].Position.z);

            Vec3Core oldEdge1 = v2 - v1;
            Vec3Core oldEdge2 = v3 - v1;
            Vec3Core oldNormal = Vec3Core::Cross(oldEdge1, oldEdge2).Normalize();

            Vec3Core newEdge1 = v2 - newPos;
            Vec3Core newEdge2 = v3 - newPos;
            Vec3Core newNormal = Vec3Core::Cross(newEdge1, newEdge2).Normalize();

            float dotProduct = oldNormal * newNormal;
            std::cout << "Triangle " << triIdx << " normal dot product: " << dotProduct << std::endl;

            if (dotProduct < 0.0f) {  // Permetti rotazioni fino a 90 gradi
                std::cout << "Rejected: Normal flip detected" << std::endl;
                return false;
            }
        }

        std::cout << "Edge collapse accepted" << std::endl;
        return true;
    }
};