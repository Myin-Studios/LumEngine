#include "RenderQueue.h"

void RenderQueue::push(const RenderCommand& command)
{
    this->_commands.push_back(command);
}

void RenderQueue::pop()
{
	this->_commands.pop_back();
}

void RenderQueue::sort()
{
    std::sort(this->_commands.begin(), this->_commands.end());
}

void RenderQueue::clear()
{
    	this->_commands.clear();
}

bool RenderQueue::empty() const
{
    return this->_commands.empty();
}

void RenderQueue::process(const glm::mat4& view, const glm::mat4& proj,
    const std::vector<glm::vec3>& lightPositions,
    const std::vector<glm::vec3>& lightColors,
    const std::vector<float>& intensities,
    const glm::vec3& camPos) {
    if (_commands.empty()) return;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Setup per il primo passaggio (mesh normale)
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xFF);

    std::shared_ptr<Material> currentMaterial = nullptr;

    // Primo passaggio: disegna oggetti normali
    for (const auto& cmd : _commands) {
        if (currentMaterial != cmd.material) {
            currentMaterial = cmd.material;
            cmd.mesh->SetMaterial(currentMaterial);
        }

        RendererDebugger::checkOpenGLError("RenderQueue::process: SetMaterial");

        auto shader = currentMaterial->GetShader();
        shader->use();

        RendererDebugger::checkOpenGLError("RenderQueue::process: Shader usage");

        // Setup trasformazioni originali
        shader->setMat4x4("model", &cmd.transform[0][0]);
        shader->setMat4x4("view", &view[0][0]);
        shader->setMat4x4("projection", &proj[0][0]);

        RendererDebugger::checkOpenGLError("RenderQueue::process: Matrices uniforms");

        if (auto pbrMat = std::dynamic_pointer_cast<PBR>(currentMaterial)) {
            shader->setVec3Array("lightPositions", lightPositions.size(), glm::value_ptr(lightPositions[0]));
            shader->setVec3Array("lightColors", lightColors.size(), glm::value_ptr(lightColors[0]));
            shader->setFloatArray("lightIntensities", intensities.size(), intensities.data());
            shader->setVec3("camPos", camPos);

            Color::Color c = Color::Color(1.0f, 1.0f, 1.0f);
            shader->setVec3("albedo", glm::vec3(c.r(), c.g(), c.b()));
            shader->setFloat("metallic", 1.0f);
            shader->setFloat("roughness", 0.4f);

            RendererDebugger::checkOpenGLError("RenderQueue::process: PBR material uniforms");
        }

        RendererDebugger::checkOpenGLError("RenderQueue::process");

        cmd.mesh->Draw();
    }

    // Setup per il secondo passaggio (outline)
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Secondo passaggio: disegna outlines
    for (const auto& cmd : _commands) {
        auto outlineMaterial = std::make_shared<Outline>();
        cmd.mesh->SetMaterial(outlineMaterial);

        auto shader = outlineMaterial->GetShader();
        shader->use();

        glm::mat4 scaledTransform = cmd.transform;
        scaledTransform = glm::scale(scaledTransform, glm::vec3(1.0f));

        shader->setMat4x4("model", &scaledTransform[0][0]);
        shader->setMat4x4("view", &view[0][0]);
        shader->setMat4x4("projection", &proj[0][0]);
        shader->setVec4("outlineColor", glm::vec4(0.0f, 0.5f, 1.0f, 1.0f));
        shader->setVec3("camPos", camPos);

        if (cmd.entity->IsSelected())
        {
            cmd.mesh->Draw();
        }

        // Ripristina il materiale originale
        cmd.mesh->SetMaterial(currentMaterial);
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Importante per vedere il wireframe

    for (const auto& cmd : _commands) {
        if (cmd.entity->IsSelected() && cmd.entity->GetCoreProperty<LumEngine::Physics::Collider>()) {
            auto collider = cmd.entity->GetCoreProperty<LumEngine::Physics::Collider>();
            AABBDebugRenderer::DrawAABB(collider->GetBoundingBox(), view, proj);
        }
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Ripristina gli stati
    glStencilMask(0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    clear();
}

const std::vector<RenderCommand>& RenderQueue::getCommands() const
{
    return this->_commands;
}
