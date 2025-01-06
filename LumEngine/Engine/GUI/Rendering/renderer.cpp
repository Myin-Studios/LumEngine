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

#include "renderer.h"
#include <QOpenGLDebugLogger>

RendererCore* RendererCore::s_instance = nullptr;

RendererCore::RendererCore(QWidget *parent)
        : QOpenGLWidget(parent)
{
    RendererCore::RegisterInstance(this);

    setAcceptDrops(true);

	this->_uiManager = std::make_unique<UIManager>();
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() { update(); });
    updateTimer->setInterval(1000/60);
}

RendererCore::~RendererCore()
{
    cleanup();

    if (_uiManager) {
        _uiManager->SetGUIBuilder(nullptr);  // Rimuoviamo il riferimento al builder
        _uiManager.reset();
    }
}

void RendererCore::initializeGL()
{
	glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(
        [](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
            std::cout << std::endl;
            std::cout << "OpenGL Debug Message: " << message << std::endl;
        },
        nullptr
    );

    RendererDebugger::checkOpenGLError("after glewInit");

    LumEngine::Physics::AABB worldBounds(Vec3Core(-10, -10, -10), Vec3Core(10, 10, 10));
    _octree = std::make_unique<LumEngine::Physics::Octree>(worldBounds);

    AABBDebugRenderer::Initialize();

    renderQueue = new RenderQueue();

    editorCamera = new Camera();

    RendererDebugger::checkOpenGLError("after camerea creation");

    mousePos = QPoint(0, 0);
    editorCamera->GetTransform()->SetPosition(0.0f, 0.0f, 3.0f);

    RendererDebugger::checkOpenGLError("after camera position");

    Light* l1 = new Light();
    l1->GetTransform()->SetPosition(3.0f, 3.0f, 3.0f);
    l1->color = Color::RGB(1.0f, 0.0f, 0.0f);
    l1->SetIntensity(50.0f);

    Light* l2 = new Light();
    l2->GetTransform()->SetPosition(-3.0f, 1.0f, 3.0f);
    l2->color = Color::RGB(0.0f, 1.0f, 0.0f);
    l2->SetIntensity(50.0f);

    Light* l3 = new Light();
    l3->GetTransform()->SetPosition(-4.0f, -2.0f, 3.0f);
    l3->color = Color::RGB(0.0f, 0.0f, 1.0f);
    l3->SetIntensity(50.0f);

    lights.push_back(*l1);
    lights.push_back(*l2);
    lights.push_back(*l3);

    setupFrameBuffer();
    // setupSkysphere();

	outline = new Outline();
    outlineShader = new Shader("Resources/Shaders/outlineVert.glsl", "Resources/Shaders/outlineFrag.glsl");

    RendererDebugger::checkOpenGLError("InitializeGL: Framebuffer setup");

    if (!GLEW_ARB_framebuffer_object) {
        qDebug() << "Framebuffer object non supportato!";
    }

}

void RendererCore::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    glBindTexture(GL_TEXTURE_2D, stencilTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

	RendererDebugger::checkOpenGLError("resizing FBO");

    update();
}

void RendererCore::paintGL()
{
    if (FBO > 0) {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    }
    else {
        qDebug() << "FBO non valido!" << FBO;
    }

    glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glm::mat4 view = glm::lookAt(
        glm::vec3(
            editorCamera->GetTransform()->GetPosition().x(),
            editorCamera->GetTransform()->GetPosition().y(),
            editorCamera->GetTransform()->GetPosition().z()),
        glm::vec3(
            editorCamera->GetTransform()->GetPosition().x() + editorCamera->GetTransform()->forward.x(),
            editorCamera->GetTransform()->GetPosition().y() + editorCamera->GetTransform()->forward.y(),
            editorCamera->GetTransform()->GetPosition().z() + editorCamera->GetTransform()->forward.z()),
        glm::vec3(
            editorCamera->GetTransform()->up.x(),
            editorCamera->GetTransform()->up.y(),
            editorCamera->GetTransform()->up.z())
    );

    glm::mat4 viewTransposed = glm::transpose(view);

    LumEngine::Physics::RayCast::SetViewMatrix(Mat4Core(
        viewTransposed[0][0], viewTransposed[0][1], viewTransposed[0][2], viewTransposed[0][3],
        viewTransposed[1][0], viewTransposed[1][1], viewTransposed[1][2], viewTransposed[1][3],
        viewTransposed[2][0], viewTransposed[2][1], viewTransposed[2][2], viewTransposed[2][3],
        viewTransposed[3][0], viewTransposed[3][1], viewTransposed[3][2], viewTransposed[3][3]
    ));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)this->width() / (float)this->height(), 0.1f, 100.0f);
    glm::mat4 projTransposed = glm::transpose(proj);

    LumEngine::Physics::RayCast::SetProjectionMatrix(Mat4Core(
        projTransposed[0][0], projTransposed[0][1], projTransposed[0][2], projTransposed[0][3],
        projTransposed[1][0], projTransposed[1][1], projTransposed[1][2], projTransposed[1][3],
        projTransposed[2][0], projTransposed[2][1], projTransposed[2][2], projTransposed[2][3],
        projTransposed[3][0], projTransposed[3][1], projTransposed[3][2], projTransposed[3][3]
    ));

    glEnable(GL_DEPTH_TEST);

    vector<glm::vec3> lightPositions;
    vector<glm::vec3> lightColors;
    std::vector<float> intensities;
    for (const auto& light : lights) {
        lightPositions.emplace_back(light.GetTransform()->GetPosition().x(),
            light.GetTransform()->GetPosition().y(),
            light.GetTransform()->GetPosition().z());

        lightColors.emplace_back(glm::vec3(
            light.color.r(),
            light.color.g(),
            light.color.b()
        ));

        intensities.push_back(light.GetIntensity());
    }

    if (!entities.empty())
    {
        for (const auto& e : entities) {
            ProcessLOD(e.get());

            if (e->GetCoreProperty<MeshCore>() != nullptr) {
                RenderCommand cmd;
                cmd.entity = e.get();
                cmd.mesh = e->GetCoreProperty<MeshCore>();
                cmd.material = e->GetCoreProperty<MeshCore>()->GetMaterial();

                glm::mat4 transform = glm::mat4(1.0f);
                if (e->GetCoreProperty<Transform3DCore>() != nullptr) {
                    auto transformCore = e->GetCoreProperty<Transform3DCore>();

                    transform = glm::translate(transform, glm::vec3(
                        transformCore->position->x(),
                        transformCore->position->y(),
                        transformCore->position->z()
                    ));

                    transform = glm::rotate(transform, glm::radians(transformCore->GetRotation().x()), glm::vec3(1, 0, 0));
                    transform = glm::rotate(transform, glm::radians(transformCore->GetRotation().y()), glm::vec3(0, 1, 0));
                    transform = glm::rotate(transform, glm::radians(transformCore->GetRotation().z()), glm::vec3(0, 0, 1));

                    transform = glm::scale(transform, glm::vec3(
                        transformCore->scale.x(),
                        transformCore->scale.y(),
                        transformCore->scale.z()
                    ));

                    if (auto collider = e->GetCoreProperty<LumEngine::Physics::Collider>()) {
                        Mat4Core transformMat = transform;
                        collider->UpdateTransform(transformMat);
                    }

                    LumEngine::Physics::RayCast::UpdateTransform(e->GetEntityID(), transform);
                }

                cmd.transform = transform;
                renderQueue->push(cmd);
            }
        }

        // Chiama process UNA SOLA VOLTA dopo aver aggiunto tutti i comandi
        renderQueue->process(view, proj, lightPositions, lightColors, intensities,
            glm::vec3(
                editorCamera->GetTransform()->GetPosition().x(),
                editorCamera->GetTransform()->GetPosition().y(),
                editorCamera->GetTransform()->GetPosition().z()
            )
        );

        // for (auto& e : entities)
        // {
        //     if (!IsRunning())
        //     {
        //         e->DeserializeProperties();
        //     }
        //     else updateTimer->start();
        // 
        //     // if (e->GetCoreProperty<MeshCore>() != nullptr)
        //     // {
        //     //     glEnable(GL_DEPTH_TEST);
        //     //     glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        //     // 
        //     //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        //     // 
        //     //     glStencilFunc(GL_ALWAYS, 1, 0xFF);
        //     //     glStencilMask(0xFF);
        //     // 
        //     //     // rendering scene with framebuffer
        //     //     e->GetCoreProperty<MeshCore>()->GetMaterial()->SetShader(std::make_unique<Shader>(*outlineShader));
        //     // 
        //     //     outlineShader->use();
        //     //     outlineShader->setMat4x4("view", &view[0][0]);
        //     //     outlineShader->setMat4x4("projection", &proj[0][0]);
        //     //     e->GetCoreProperty<MeshCore>()->Draw();
        //     // 
        //     //     if (e->GetCoreProperty<Transform3DCore>() != nullptr)
        //     //     {
        //     //         glm::mat4 tMat = glm::mat4(1.0f); // Inizia con una matrice identità
        //     //         tMat = glm::translate(tMat, glm::vec3(
        //     //             e->GetCoreProperty<Transform3DCore>()->position->x(),
        //     //             e->GetCoreProperty<Transform3DCore>()->position->y(),
        //     //             e->GetCoreProperty<Transform3DCore>()->position->z()
        //     //         ));
        //     //         tMat = glm::scale(tMat, glm::vec3(
        //     //             e->GetCoreProperty<Transform3DCore>()->scale.x(),
        //     //             e->GetCoreProperty<Transform3DCore>()->scale.y(),
        //     //             e->GetCoreProperty<Transform3DCore>()->scale.z()
        //     //         ));
        //     // 
        //     //         outlineShader->setMat4x4("model", &tMat[0][0]);
        //     //     }
        //     //     else
        //     //     {
        //     //         outlineShader->setMat4x4("model", &glm::mat4(1.0f)[0][0]);
        //     //     }
        //     // 
        //     //     glDisable(GL_STENCIL_TEST);
        //     // 
        //     //     // rendering normal scene
        //     //     e->GetCoreProperty<MeshCore>()->SetMaterial(std::make_shared<PBR>());
        //     // 
        //     //     e->GetCoreProperty<MeshCore>()->Draw();
        //     // 
        //     //     if (e->GetCoreProperty<Transform3DCore>() != nullptr)
        //     //     {
        //     //         glm::mat4 tMat = glm::mat4(1.0f); // Inizia con una matrice identità
        //     //         tMat = glm::translate(tMat, glm::vec3(
        //     //             e->GetCoreProperty<Transform3DCore>()->position->x(),
        //     //             e->GetCoreProperty<Transform3DCore>()->position->y(),
        //     //             e->GetCoreProperty<Transform3DCore>()->position->z()
        //     //         ));
        //     //         tMat = glm::scale(tMat, glm::vec3(
        //     //             e->GetCoreProperty<Transform3DCore>()->scale.x(),
        //     //             e->GetCoreProperty<Transform3DCore>()->scale.y(),
        //     //             e->GetCoreProperty<Transform3DCore>()->scale.z()
        //     //         ));
        //     // 
        //     //         e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("model", &tMat[0][0]);
        //     //     }
        //     //     else
        //     //     {
        //     //         e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("model", &glm::mat4(1.0)[0][0]);
        //     //     }
        //     // 
        //     //     e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("view", &view[0][0]);
        //     // 
        //     //     e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("projection", &proj[0][0]);
        //     // 
        //     //     vector<glm::vec3> lightPositions;
        //     //     vector<glm::vec3> lightColors;
        //     //     std::vector<float> intensities;
        //     //     for (const auto& light : lights) {
        //     //         lightPositions.emplace_back(light.GetTransform()->GetPosition().x(),
        //     //             light.GetTransform()->GetPosition().y(),
        //     //             light.GetTransform()->GetPosition().z());
        //     // 
        //     //         lightColors.emplace_back(glm::vec3(
        //     //             light.color.r(),
        //     //             light.color.g(),
        //     //             light.color.b()
        //     //         ));
        //     // 
        //     //         intensities.push_back(light.GetIntensity());
        //     //     }
        //     // 
        //     //     e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setVec3Array("lightPositions", lightPositions.size(), glm::value_ptr(lightPositions[0]));
        //     //     e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setVec3Array("lightColors", lightColors.size(), glm::value_ptr(lightColors[0]));
        //     //     e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setFloatArray("lightIntensities", intensities.size(), intensities.data());
        //     //     e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setVec3("camPos", glm::vec3(
        //     //         editorCamera->GetTransform()->GetPosition().x(),
        //     //         editorCamera->GetTransform()->GetPosition().y(),
        //     //         editorCamera->GetTransform()->GetPosition().z()
        //     //     ));
        //     // 
        //     //     if (auto pbrMat = std::dynamic_pointer_cast<PBR>(e->GetCoreProperty<MeshCore>()->GetMaterial())) {
        //     //         Color::Color c(1.0f, 0.5f, 0.0f);
        //     //         pbrMat->SetAlbedo(c);
        //     //         pbrMat->SetMetallic(1.0f);
        //     //         pbrMat->SetRoughness(0.4f);
        //     //     }
        //     // 
        //     //     glUseProgram(0);
        //     // }
        // 
        //     glEnable(GL_DEPTH_TEST);
        //     glEnable(GL_STENCIL_TEST);
        // 
        //     // Pulizia dei buffer
        //     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        // 
        //     // Disegno della mesh principale
        //     if (e->GetCoreProperty<MeshCore>() != nullptr)
        //     {
        //         auto mesh = e->GetCoreProperty<MeshCore>();
        //         auto transform = e->GetCoreProperty<Transform3DCore>();
        // 
        //         // STEP 1: Disegno normale della mesh
        //         glStencilMask(0xFF); // Consenti aggiornamenti del buffer stencil
        //         glStencilFunc(GL_ALWAYS, 1, 0xFF); // Ogni frammento aggiorna il buffer stencil
        // 
        //         auto shader = mesh->GetMaterial()->GetShader();
        //         shader->use();
        // 
        //         // Calcolo della matrice di trasformazione
        //         glm::mat4 modelMat = glm::mat4(1.0f);
        //         if (transform != nullptr)
        //         {
        //             modelMat = glm::translate(modelMat, glm::vec3(
        //                 transform->position->x(),
        //                 transform->position->y(),
        //                 transform->position->z()
        //             ));
        //             modelMat = glm::scale(modelMat, glm::vec3(
        //                 transform->scale.x(),
        //                 transform->scale.y(),
        //                 transform->scale.z()
        //             ));
        //         }
        //         shader->setMat4x4("model", &modelMat[0][0]);
        //         shader->setMat4x4("view", &view[0][0]);
        //         shader->setMat4x4("projection", &proj[0][0]);
        // 
        //         // Disegna la mesh principale
        //         mesh->Draw();
        // 
        //         // STEP 2: Disegno del contorno
        //         glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Disegna solo dove lo stencil buffer NON è 1
        //         glStencilMask(0x00); // Blocca aggiornamenti del buffer stencil
        //         glDisable(GL_DEPTH_TEST); // Disabilita il test di profondità per l'outline
        // 
        //         outlineShader->use();
        // 
        //         // Configurazione per il disegno dell'outline (leggermente ingrandito)
        //         glm::mat4 outlineMat = glm::scale(modelMat, glm::vec3(1.1f, 1.1f, 1.1f));
        //         outlineShader->setMat4x4("model", &outlineMat[0][0]);
        //         outlineShader->setMat4x4("view", &view[0][0]);
        //         outlineShader->setMat4x4("projection", &proj[0][0]);
        // 
        //         mesh->Draw();
        // 
        //         // Ripristina le impostazioni di default
        //         glStencilMask(0xFF); // Consenti aggiornamenti del buffer stencil
        //         glStencilFunc(GL_ALWAYS, 1, 0xFF); // Torna alla modalità di default
        //         glEnable(GL_DEPTH_TEST); // Riattiva il test di profondità
        //     }
        // }
    }

    // Seconda fase: rendering del quad sullo schermo

    // Rendering phase
    //if (skysphere != nullptr)
    //{
    //    glDisable(GL_DEPTH_TEST);
    //
    //    skysphere->Draw();
    //
    //    glm::mat4 skyboxView = glm::lookAt(
    //        glm::vec3(
    //            editorCamera->GetTransform()->GetPosition().x(),
    //            editorCamera->GetTransform()->GetPosition().y(),
    //            editorCamera->GetTransform()->GetPosition().z()),
    //        glm::vec3(
    //            editorCamera->GetTransform()->GetPosition().x() + editorCamera->GetTransform()->forward.x(),
    //            editorCamera->GetTransform()->GetPosition().y() + editorCamera->GetTransform()->forward.y(),
    //            editorCamera->GetTransform()->GetPosition().z() + editorCamera->GetTransform()->forward.z()),
    //        glm::vec3(
    //            editorCamera->GetTransform()->up.x(),
    //            editorCamera->GetTransform()->up.y(),
    //            editorCamera->GetTransform()->up.z())
    //    );
    //
    //    skysphere->GetMaterial()->GetShader()->setMat4x4("model", &glm::translate(glm::mat4(1.0f), glm::vec3(
    //        editorCamera->GetTransform()->GetPosition().x(),
    //        editorCamera->GetTransform()->GetPosition().y(),
    //        editorCamera->GetTransform()->GetPosition().z()
    //    ))[0][0]);
    //    skysphere->GetMaterial()->GetShader()->setMat4x4("view", &skyboxView[0][0]);
    //    skysphere->GetMaterial()->GetShader()->setMat4x4("projection", &glm::perspective(glm::radians(45.0f), (float)this->width() / (float)this->height(), 0.1f, 100.0f)[0][0]);
    //}
    glUseProgram(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 2);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (canUpdateCamera)
    {
        UpdateCamera();
    }

    fboShader->use();
    glBindVertexArray(screenVAO);
    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    fboShader->setInt("screenTexture", 0);

    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, depthTexture);
    // fboShader->setInt("depthTexture", 1);
    // 
    // fboShader->setFloat("width", static_cast<float>(width()));
    // fboShader->setFloat("height", static_cast<float>(height()));

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glUseProgram(0);
}

void RendererCore::UpdateCamera()
{
    if (keyPressed.find(Qt::Key_A) != keyPressed.end())
        editorCamera->GetTransform()->Move(new Vec3Core(editorCamera->GetTransform()->right.Normalize() * -0.1f));
    if (keyPressed.find(Qt::Key_D) != keyPressed.end())
        editorCamera->GetTransform()->Move(new Vec3Core(editorCamera->GetTransform()->right.Normalize() * 0.1f));
    if (keyPressed.find(Qt::Key_W) != keyPressed.end())
        editorCamera->GetTransform()->Move(new Vec3Core(editorCamera->GetTransform()->forward.Normalize() * 0.1f));
    if (keyPressed.find(Qt::Key_S) != keyPressed.end())
        editorCamera->GetTransform()->Move(new Vec3Core(editorCamera->GetTransform()->forward.Normalize() * -0.1f));
    if (keyPressed.find(Qt::Key_Q) != keyPressed.end())
        editorCamera->GetTransform()->Move(new Vec3Core(editorCamera->GetTransform()->up.Normalize() * -0.05f));
    if (keyPressed.find(Qt::Key_E) != keyPressed.end())
        editorCamera->GetTransform()->Move(new Vec3Core(editorCamera->GetTransform()->up.Normalize() * 0.05f));
}

void RendererCore::ProcessLOD(BaseEntity* e)
{
    if (!e) return;  // Controllo di sicurezza sull'entità

    auto mesh = e->GetCoreProperty<MeshCore>();
    if (!mesh) return;  // Controllo se esiste il MeshCore

    auto lodSettings = mesh->GetLODSettings();
    if (!lodSettings) return;  // Controllo se esistono le impostazioni LOD

    auto transform = e->GetCoreProperty<Transform3DCore>();
    if (!transform) return;

    // Calcola la distanza dalla camera all'oggetto
    float distance = (editorCamera->GetTransform()->GetPosition() -
        transform->GetPosition()).Length();

    // Imposta i limiti per il LOD
    int targetLOD;
    if (distance > lodSettings->GetMaxDistance()) {
        targetLOD = lodSettings->GetSteps();
    }
    else if (distance < lodSettings->GetMinDistance()) {
        targetLOD = 0;
    }
    else {
        // Interpola il livello di LOD basato sulla distanza
        float t = (distance - lodSettings->GetMinDistance()) /
            (lodSettings->GetMaxDistance() - lodSettings->GetMinDistance());
        targetLOD = static_cast<int>(t * lodSettings->GetSteps());
        // Assicurati che il livello sia nei limiti
        targetLOD = std::clamp(targetLOD, 0, lodSettings->GetSteps());
    }

    // Aggiorna il LOD solo se necessario
    if (mesh->GetCurrentLOD() != targetLOD) {  // Confronta con il livello corrente
        mesh->SetLOD(targetLOD);
    }
}

void RendererCore::setupSkysphere()
{
    skysphere = LoadOBJ("Resources/Models/Skysphere.obj", std::make_shared<ProceduralSkybox>());
}

void RendererCore::checkFrameBufferError()
{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        switch(status) {
            case GL_FRAMEBUFFER_UNDEFINED:
                std::cout << "GL_FRAMEBUFFER_UNDEFINED" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                std::cout << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                std::cout << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                std::cout << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                std::cout << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                std::cout << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                std::cout << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << std::endl;
                break;
            default:
                std::cout << "Unknown framebuffer error: " << status << std::endl;
                break;
        }
    }
}

void RendererCore::setupFrameBuffer()
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	RendererDebugger::checkOpenGLError("setting up FBO");

    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width(), height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    checkFrameBufferError();
    
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width(), height(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    checkFrameBufferError();
    
    glGenTextures(1, &stencilTexture);
    glBindTexture(GL_TEXTURE_2D, stencilTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width(), height(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

	RendererDebugger::checkOpenGLError("setting up FBO texture");

    checkFrameBufferError();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	RendererDebugger::checkOpenGLError("unbinding FBO in the FBO setup method");

    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &screenVBO);

    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);

    if (!screenQuad.empty()) {
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * screenQuad.size(), screenQuad.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    }

    glBindVertexArray(0);

    checkFrameBufferError();

    RendererDebugger::checkOpenGLError("setting up screen buffers");

    fboShader = new Shader("Resources/Shaders/fboVertex.glsl", "Resources/Shaders/fboFragment.glsl");
}

void RendererCore::cleanup()
{
    makeCurrent();

    doneCurrent();
}

void RendererCore::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("application/x-lume-asset"))
    {
        event->acceptProposedAction();
    }
}

void RendererCore::dropEvent(QDropEvent *event) {
    const QMimeData* mimeData = event->mimeData();
	if (mimeData->hasUrls()) {
		QList<QUrl> urlList = mimeData->urls();
		for (const QUrl& url : urlList) {
			QString filePath = url.toLocalFile();
			QFileInfo fileInfo(filePath);

			if (fileInfo.suffix() == "obj" || fileInfo.suffix() == "fbx") {
				loadModel(filePath);
			}
		}
	}
	else if (mimeData->hasText()) {
		QString filePath = event->mimeData()->text();
		QFileInfo fileInfo(filePath);

		if (fileInfo.suffix() == "obj" || fileInfo.suffix() == "fbx") {
			loadModel(filePath);
		}
	}
}

void RendererCore::mousePressEvent(QMouseEvent* event) {
    setFocus();
    if (event->button() == Qt::RightButton) {
        setCursor(Qt::ClosedHandCursor);
        canUpdateCamera = true;
        mousePos = event->pos();
        updateTimer->start();
    }
    else if (event->button() == Qt::LeftButton) {
        Vec3Core rayOrigin = editorCamera->GetTransform()->GetPosition();
        Vec3Core rayDir = LumEngine::Physics::RayCast::ScreenToRay(
            event->pos().x(),
            event->pos().y(),
            width(), height());

        std::cout << "Mouse click at: " << event->pos().x() << ", " << event->pos().y() << std::endl;
        std::cout << "Camera position: " << rayOrigin.ToString() << std::endl;
        std::cout << "Ray direction: " << rayDir.ToString() << std::endl;

        BaseEntity* selectedEntity = _octree->RayCast(rayOrigin, rayDir);

        if (selectedEntity) {
            std::cout << "Selected Entity: " << selectedEntity->GetEntityID() << std::endl;
            selectedEntity->SetSelected(true);

            if (this->_uiManager) {
                this->_uiManager->UpdateUI(selectedEntity);
            }
            else {
                std::cerr << "UI Manager is not initialized." << std::endl;
            }
            update();
        }
        else {
            if (this->_uiManager) this->_uiManager->UpdateUI();
            for (auto& e : entities) {
                e->SetSelected(false);
            }
            update();
        }
    }
}

void RendererCore::mouseMoveEvent(QMouseEvent* event)
{
    QPoint currentPos = event->pos();

    float deltaX = 0.0f;
    float deltaY = 0.0f;

    if (event->buttons() & Qt::RightButton)
    {
        deltaX = currentPos.x() - mousePos.x();
        deltaY = currentPos.y() - mousePos.y();

        editorCamera->GetTransform()->Rotate(deltaX * 0.1f, -deltaY * 0.1f, 0.0f);

        mousePos = currentPos;
    }
}

void RendererCore::mouseReleaseEvent(QMouseEvent* event)
{
    setCursor(Qt::ArrowCursor);

    if (event->button() == Qt::RightButton)
    {
        canUpdateCamera = false;
        updateTimer->stop();

        keyPressed.clear();
    }
}

void RendererCore::keyPressEvent(QKeyEvent* event)
{
    if (canUpdateCamera)
    {
        if (event->key() == Qt::Key_W)
        {
            keyPressed.emplace(event->key());
        }
        if (event->key() == Qt::Key_A)
        {
            keyPressed.emplace(event->key());
        }
        if (event->key() == Qt::Key_S)
        {
            keyPressed.emplace(event->key());
        }
        if (event->key() == Qt::Key_D)
        {
            keyPressed.emplace(event->key());
        }
        if (event->key() == Qt::Key_Q)
        {
            keyPressed.emplace(event->key());
        }
        if (event->key() == Qt::Key_E)
        {
            keyPressed.emplace(event->key());
        }

		// Uncomment when the delete functionality is implemented (when the octree mesh removal is implemented)

		// auto it = find_if(entities.begin(), entities.end(), [](const shared_ptr<BaseEntity>& e) { return e->IsSelected(); });
        // 
		// if (it != entities.end())
        // {
        //     if (event->key() == Qt::Key_Delete)
        //     {
		// 		_octree->Remove((*it).get());
		// 		entities.erase(it);
		// 	}
		// }

        update();
    }
}

void RendererCore::keyReleaseEvent(QKeyEvent* event)
{
    if (canUpdateCamera)
    {
        keyPressed.erase(event->key());
    }
}

void RendererCore::loadModel(const QString& path) {
    if (path.endsWith(".obj"))
    {
        entities.emplace_back(std::make_shared<BaseEntity>());
        entities.back()->AddProperty<MeshCore>(MeshCache::GetInstance()->LoadOrGetModel(path.toStdString()));

        Mat4Core t = {};

        entities.back()->AddProperty(std::make_unique<LumEngine::Physics::Collider>(
            entities.back()->GetEntityID(), entities.back()->GetCoreProperty<MeshCore>()->GetVertices(), t));
        entities.back()->AddProperty<Transform3DCore>(std::make_unique<Transform3DCore>());

		// entities.back()->AddProperty<LumEngine::Graphics::LODCore>(std::make_unique<LumEngine::Graphics::LODCore>(3));

        LumEngine::Physics::RayCast::RegisterBoundingVolume(
            entities.back()->GetCoreProperty<MeshCore>()->GetVertices()
        );

        if (auto mesh = entities.back()->GetCoreProperty<MeshCore>()) {
            // First, get the existing LOD settings
            if (auto lodCore = entities.back()->GetCoreProperty<LumEngine::Graphics::LODCore>()) {
                // Create a new LODCore using the values from the existing one
                mesh->SetLODSettings(std::make_shared<LumEngine::Graphics::LODCore>(
                    lodCore->GetSteps(),
                    lodCore->GetMaxDistance(),
                    lodCore->GetMinDistance()
                ));
            }
            else {
                // If no LODCore exists, create one with default values
                mesh->SetLODSettings(std::make_shared<LumEngine::Graphics::LODCore>());
            }
            mesh->SetLOD(0);
        }

		_octree->Insert(entities.back().get());
    }
}

std::unique_ptr<MeshCore> RendererCore::LoadOBJ(const std::string& path, std::shared_ptr<Material> mat) {
    std::ifstream file(path);
    if (!file.is_open()) {
        qCritical() << "Unable to open the file:" << path;
        return nullptr;
    }

    std::vector<Vertex> ver;
    std::vector<unsigned int> ids;
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;

    // Pre-allocazione per evitare riallocazioni frequenti
    size_t fileSize = file.tellg();
    size_t estimatedVertices = fileSize / 100;
    vertices.reserve(estimatedVertices);
    normals.reserve(estimatedVertices);
    texCoords.reserve(estimatedVertices);
    ver.reserve(estimatedVertices);
    ids.reserve(estimatedVertices * 3);

    // Iteratori per il file
    std::istreambuf_iterator<char> begin(file), end;
    std::string token;
    std::string number;
    number.reserve(20);

    for (auto it = begin; it != end; ++it) {
        char c = *it;

        // Salta spazi bianchi e commenti
        if (std::isspace(c)) continue;
        if (c == '#') {
            while (it != end && *it != '\n') ++it;
            continue;
        }

        // Leggi il tipo (v, vt, vn, f)
        token.clear();
        while (it != end && !std::isspace(*it)) {
            token += *it;
            ++it;
        }

        if (token == "v") {
            glm::vec3 pos;
            // Leggi x
            number.clear();
            while (it != end && std::isspace(*it)) ++it;
            while (it != end && !std::isspace(*it)) {
                number += *it;
                ++it;
            }
            if (!number.empty()) pos.x = std::stof(number);

            // Leggi y
            number.clear();
            while (it != end && std::isspace(*it)) ++it;
            while (it != end && !std::isspace(*it)) {
                number += *it;
                ++it;
            }
            if (!number.empty()) pos.y = std::stof(number);

            // Leggi z
            number.clear();
            while (it != end && std::isspace(*it)) ++it;
            while (it != end && !std::isspace(*it)) {
                number += *it;
                ++it;
            }
            if (!number.empty()) pos.z = std::stof(number);

            vertices.push_back(pos);
        }
        else if (token == "vt") {
            glm::vec2 uv;
            // Leggi u
            number.clear();
            while (it != end && std::isspace(*it)) ++it;
            while (it != end && !std::isspace(*it)) {
                number += *it;
                ++it;
            }
            if (!number.empty()) uv.x = std::stof(number);

            // Leggi v
            number.clear();
            while (it != end && std::isspace(*it)) ++it;
            while (it != end && !std::isspace(*it)) {
                number += *it;
                ++it;
            }
            if (!number.empty()) uv.y = std::stof(number);

            texCoords.push_back(uv);
        }
        else if (token == "vn") {
            glm::vec3 normal;
            // Leggi nx
            number.clear();
            while (it != end && std::isspace(*it)) ++it;
            while (it != end && !std::isspace(*it)) {
                number += *it;
                ++it;
            }
            if (!number.empty()) normal.x = std::stof(number);

            // Leggi ny
            number.clear();
            while (it != end && std::isspace(*it)) ++it;
            while (it != end && !std::isspace(*it)) {
                number += *it;
                ++it;
            }
            if (!number.empty()) normal.y = std::stof(number);

            // Leggi nz
            number.clear();
            while (it != end && std::isspace(*it)) ++it;
            while (it != end && !std::isspace(*it)) {
                number += *it;
                ++it;
            }
            if (!number.empty()) normal.z = std::stof(number);

            normals.push_back(normal);
        }
        else if (token == "f") {
            // Parser per le facce
            for (int i = 0; i < 3; ++i) {
                int vertexIndex = -1, uvIndex = -1, normalIndex = -1;

                // Leggi indice vertice
                number.clear();
                while (it != end && std::isspace(*it)) ++it;
                while (it != end && *it != '/' && !std::isspace(*it)) {
                    number += *it;
                    ++it;
                }
                if (!number.empty()) vertexIndex = std::stoi(number);

                // Se c'è una texture coordinate
                if (it != end && *it == '/') {
                    ++it;
                    number.clear();
                    while (it != end && *it != '/' && !std::isspace(*it)) {
                        number += *it;
                        ++it;
                    }
                    if (!number.empty()) uvIndex = std::stoi(number);
                }

                // Se c'è una normale
                if (it != end && *it == '/') {
                    ++it;
                    number.clear();
                    while (it != end && !std::isspace(*it)) {
                        number += *it;
                        ++it;
                    }
                    if (!number.empty()) normalIndex = std::stoi(number);
                }

                if (vertexIndex > 0 && vertexIndex <= vertices.size()) {
                    ver.emplace_back(
                        vertices[vertexIndex - 1],
                        normalIndex > 0 && normalIndex <= normals.size() ? normals[normalIndex - 1] : glm::vec3(0),
                        uvIndex > 0 && uvIndex <= texCoords.size() ? texCoords[uvIndex - 1] : glm::vec2(0)
                    );
                    ids.push_back(ver.size() - 1);
                }
            }
        }
    }

    file.close();

    if (ver.empty() || ids.empty()) {
        qWarning() << "No valid geometry found in OBJ file";
        return nullptr;
    }

    makeCurrent();
    auto m = std::make_unique<MeshCore>(std::move(ver), std::move(ids), path);
    m->SetMaterial(mat);

    update();

    return m;
}

extern "C" {
    RendererCore* GetEngineRenderer() {
        return RendererCore::GetInstance();
    }

    BaseEntity* GetEngineEntityAt(int index) {
        if (RendererCore::GetInstance() && index >= 0 && index < RendererCore::GetInstance()->GetEntities().size()) {
            return RendererCore::GetInstance()->GetEntities()[index].get();
        }
        return nullptr;
    }

    int GetEngineEntityCount() {
        auto renderer = RendererCore::GetInstance();
        return renderer ? renderer->GetEntities().size() : 0;
    }
}

// MESH CACHING
MeshCache* MeshCache::s_instance = nullptr;

std::unique_ptr<MeshCore> MeshCache::LoadOrGetModel(const std::string& path)
{
    auto it = _meshes.find(path);

    if (it != _meshes.end()) {
        return std::make_unique<MeshCore>(*it->second);
    }

    auto model = LoadOBJ(path, std::make_shared<PBR>());
    if (model) {
        _meshes[path] = std::make_unique<MeshCore>(*model);
        RendererCore::GetInstance()->update();
    }
    return model;
}

std::unique_ptr<MeshCore> MeshCache::LoadOBJ(const std::string& path, std::shared_ptr<Material> mat)
{
    return RendererCore::GetInstance()->LoadOBJ(path, mat);
}

