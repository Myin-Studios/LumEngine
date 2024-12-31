#pragma once

#include <GL/glew.h>
#include "../LumTypes/Physics/Collisions.h"
#include "Engine/GUI/Rendering/Meshes/Mesh.h"

class AABBDebugRenderer {
public:
    static std::unique_ptr<MeshCore> debugMesh;
    static std::shared_ptr<Material> debugMaterial;

    static void Initialize() {
        // Crea i vertici per il box wireframe - ogni coppia di vertici forma una linea
        vector<Vertex> boxVertices = {
            // Linee frontali
            {{-0.5f, -0.5f, -0.5f}, {0,0,0}, {0,0}},
            {{ 0.5f, -0.5f, -0.5f}, {0,0,0}, {0,0}},

            {{ 0.5f, -0.5f, -0.5f}, {0,0,0}, {0,0}},
            {{ 0.5f,  0.5f, -0.5f}, {0,0,0}, {0,0}},

            {{ 0.5f,  0.5f, -0.5f}, {0,0,0}, {0,0}},
            {{-0.5f,  0.5f, -0.5f}, {0,0,0}, {0,0}},

            {{-0.5f,  0.5f, -0.5f}, {0,0,0}, {0,0}},
            {{-0.5f, -0.5f, -0.5f}, {0,0,0}, {0,0}},

            // Linee posteriori
            {{-0.5f, -0.5f,  0.5f}, {0,0,0}, {0,0}},
            {{ 0.5f, -0.5f,  0.5f}, {0,0,0}, {0,0}},

            {{ 0.5f, -0.5f,  0.5f}, {0,0,0}, {0,0}},
            {{ 0.5f,  0.5f,  0.5f}, {0,0,0}, {0,0}},

            {{ 0.5f,  0.5f,  0.5f}, {0,0,0}, {0,0}},
            {{-0.5f,  0.5f,  0.5f}, {0,0,0}, {0,0}},

            {{-0.5f,  0.5f,  0.5f}, {0,0,0}, {0,0}},
            {{-0.5f, -0.5f,  0.5f}, {0,0,0}, {0,0}},

            // Linee di connessione
            {{-0.5f, -0.5f, -0.5f}, {0,0,0}, {0,0}},
            {{-0.5f, -0.5f,  0.5f}, {0,0,0}, {0,0}},

            {{ 0.5f, -0.5f, -0.5f}, {0,0,0}, {0,0}},
            {{ 0.5f, -0.5f,  0.5f}, {0,0,0}, {0,0}},

            {{ 0.5f,  0.5f, -0.5f}, {0,0,0}, {0,0}},
            {{ 0.5f,  0.5f,  0.5f}, {0,0,0}, {0,0}},

            {{-0.5f,  0.5f, -0.5f}, {0,0,0}, {0,0}},
            {{-0.5f,  0.5f,  0.5f}, {0,0,0}, {0,0}}
        };

        debugMesh = std::make_unique<MeshCore>(boxVertices);
        debugMaterial = std::make_shared<Material>(std::make_unique<Shader>(
            "Resources/Shaders/AABBDebuggerVert.glsl",
            "Resources/Shaders/AABBDebuggerFrag.glsl"
        ));
        debugMesh->SetMaterial(debugMaterial);
    }

    static void DrawAABB(const LumEngine::Physics::AABB& aabb, const glm::mat4& view, const glm::mat4& proj) {
        if (!debugMesh || !debugMaterial || !debugMaterial->GetShader()) return;

        Vec3Core min = aabb.GetMin();
        Vec3Core max = aabb.GetMax();

        auto shader = debugMaterial->GetShader();
        shader->use();

        glm::mat4 model = glm::translate(glm::mat4(1.0f),
            glm::vec3((min.x() + max.x()) * 0.5f,
                (min.y() + max.y()) * 0.5f,
                (min.z() + max.z()) * 0.5f));

        model = glm::scale(model, glm::vec3(
            max.x() - min.x(),
            max.y() - min.y(),
            max.z() - min.z()));

        shader->setMat4x4("model", &model[0][0]);
        shader->setMat4x4("view", &view[0][0]);
        shader->setMat4x4("projection", &proj[0][0]);

		debugMesh->DrawWireframe();
    }
};

inline std::unique_ptr<MeshCore> AABBDebugRenderer::debugMesh;
inline std::shared_ptr<Material> AABBDebugRenderer::debugMaterial;