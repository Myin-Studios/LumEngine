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

    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, [this]() { update(); });
    updateTimer->setInterval(1000/60);
}

RendererCore::~RendererCore()
{
    cleanup();
}

void RendererCore::initializeGL()
{

    // QOpenGLDebugLogger* logger = new QOpenGLDebugLogger(this);
    // if (logger->initialize()) {
    //     logger->startLogging(QOpenGLDebugLogger::SynchronousLogging);
    //     connect(logger, &QOpenGLDebugLogger::messageLogged, this,
    //         [](const QOpenGLDebugMessage& msg) {
    //             qDebug() << msg;
    //         });
    // }

    RendererDebugger::checkOpenGLError("after initializeOpenGLFunctions");

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

    qDebug() << "GLEW version:" << QString((const char*)glewGetString(GLEW_VERSION));
    qDebug() << "OpenGL version:" << QString((const char*)glGetString(GL_VERSION));

    RendererDebugger::checkOpenGLError("after glewInit");

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
        // Attiva stencil e depth test
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);

        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);  // Scrivi 1 nello stencil buffer
        glStencilMask(0xFF);

        for (const auto& e : entities)
        {
            if (!IsRunning())
            {
                e->DeserializeProperties();
            }
            else updateTimer->start();

            if (e->GetCoreProperty<MeshCore>() != nullptr)
            {
                // Configura shader e materiale
                e->GetCoreProperty<MeshCore>()->SetMaterial(std::make_shared<PBR>());
                e->GetCoreProperty<MeshCore>()->Draw();

                if (e->GetCoreProperty<Transform3DCore>() != nullptr)
                {
                    glm::mat4 tMat = glm::mat4(1.0f);
                    tMat = glm::translate(tMat, glm::vec3(
                        e->GetCoreProperty<Transform3DCore>()->position->x(),
                        e->GetCoreProperty<Transform3DCore>()->position->y(),
                        e->GetCoreProperty<Transform3DCore>()->position->z()
                    ));
                    tMat = glm::scale(tMat, glm::vec3(
                        e->GetCoreProperty<Transform3DCore>()->scale.x(),
                        e->GetCoreProperty<Transform3DCore>()->scale.y(),
                        e->GetCoreProperty<Transform3DCore>()->scale.z()
                    ));

                    e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("model", &tMat[0][0]);
                }

                // Configura altre uniformi per il rendering
                e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("view", &view[0][0]);
                e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("projection", &proj[0][0]);
            }
        }

        // Secondo passaggio: Disegno del contorno
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  // Disegna solo dove lo stencil non è 1
        glStencilMask(0x00);                  // Blocca aggiornamenti sullo stencil buffer
        glDisable(GL_DEPTH_TEST);             // Disabilita il depth test per il contorno

        for (const auto& e : entities)
        {
            if (!IsRunning())
            {
                e->DeserializeProperties();
            }
            else updateTimer->start();

            if (e->GetCoreProperty<MeshCore>() != nullptr)
            {
                // Secondo passaggio: Renderizza l'outline
                glStencilFunc(GL_NOTEQUAL, 1, 0xFF); // Renderizza solo dove lo stencil non è 1
                glStencilMask(0x00);                 // Blocca aggiornamenti sul buffer stencil
                glDisable(GL_DEPTH_TEST);            // Disabilita il test di profondità

                e->GetCoreProperty<MeshCore>()->GetMaterial()->SetShader(std::make_unique<Shader>(*outlineShader));

                e->GetCoreProperty<MeshCore>()->Draw();

                glm::mat4 outlineMat = glm::mat4(1.0f);
                if (e->GetCoreProperty<Transform3DCore>() != nullptr)
                {
                    outlineMat = glm::translate(outlineMat, glm::vec3(
                        e->GetCoreProperty<Transform3DCore>()->position->x(),
                        e->GetCoreProperty<Transform3DCore>()->position->y(),
                        e->GetCoreProperty<Transform3DCore>()->position->z()
                    ));
                    outlineMat = glm::scale(outlineMat, glm::vec3(
                        e->GetCoreProperty<Transform3DCore>()->scale.x() * 1.05f,
                        e->GetCoreProperty<Transform3DCore>()->scale.y() * 1.05f,
                        e->GetCoreProperty<Transform3DCore>()->scale.z() * 1.05f
                    ));
                }

                e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("model", &outlineMat[0][0]);
                e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("view", &view[0][0]);
                e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("projection", &proj[0][0]);
            }
        }

        glDisable(GL_STENCIL_TEST);

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
    // fboShader->setInt("inputTexture", 0);

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

void RendererCore::setupSkysphere()
{
    skysphere = loadOBJ("Resources/Models/Skysphere.obj", std::make_shared<ProceduralSkybox>());
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
    qDebug() << "Generated FBO:" << FBO;

	std::cout << "Width: " << width() << "\nHeight: " << height() << std::endl;

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
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void RendererCore::dropEvent(QDropEvent *event) {
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.empty())
        {
            QString fileName = urlList.at(0).toLocalFile();

            if (fileName.contains(".obj"))
            {
                loadModel(fileName);
            }
        }
    }
}

void RendererCore::mousePressEvent(QMouseEvent* event)
{
    setFocus();

    if (event->button() == Qt::RightButton)
    {
        setCursor(Qt::ClosedHandCursor);

        canUpdateCamera = true;

        mousePos = event->pos();

        updateTimer->start();
    }
    else if (event->button() == Qt::LeftButton)
    {
		Vec3Core rOrigin, rDirection;

        LumEngine::Physics::RayCast::ScreenToRay(
            event->pos().x(),
            event->pos().y(),
            width(), height(),
            rOrigin, rDirection);

        LumEngine::Physics::RayCastResult res = LumEngine::Physics::RayCast::Cast(
            rOrigin,
            rDirection);

        if (res.hit)
        {
            std::cout << "Hit!" << std::endl;
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
        entities.back()->AddProperty<MeshCore>(loadOBJ(path, std::make_shared<PBR>()));
        entities.back()->AddProperty(std::make_unique<LumEngine::Physics::Collider>(
            entities.back()->GetEntityID(), entities.back()->GetCoreProperty<MeshCore>()->GetVertices()));
        entities.back()->AddProperty<Transform3DCore>(std::make_unique<Transform3DCore>());

        std::cout << entities.back()->GetCoreProperty<MeshCore>()->GetVertices().back().Position.x << std::endl;

        LumEngine::Physics::RayCast::RegisterBoundingVolume(
            entities.back()->GetCoreProperty<MeshCore>()->GetVertices()
        );
    }
}

std::unique_ptr<MeshCore> RendererCore::loadOBJ(const QString& path, std::shared_ptr<Material> mat) {
    std::ifstream file(path.toStdString());
    if (!file.is_open()) {
        qCritical() << "Unable to open the file:" << path;
        return nullptr;
    }

    std::vector<Vertex> ver;
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> texCoords;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; // Salta linee vuote o commenti

        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            glm::vec3 pos;
            iss >> pos.x >> pos.y >> pos.z;
            vertices.push_back(pos);
        }
        else if (prefix == "vt") {
            glm::vec2 uv;
            iss >> uv.x >> uv.y;
            texCoords.push_back(uv);
        }
        else if (prefix == "vn") {
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (prefix == "f") {
            for (int i = 0; i < 3; ++i) {
                std::string vertexData;
                iss >> vertexData;

                int vertexIndex = -1, uvIndex = -1, normalIndex = -1;
                std::replace(vertexData.begin(), vertexData.end(), '/', ' '); // Sostituisce '/' con spazio
                std::istringstream vertexStream(vertexData);
                vertexStream >> vertexIndex >> uvIndex >> normalIndex;

                ver.emplace_back(
                    vertexIndex > 0 ? vertices[vertexIndex - 1] : glm::vec3(0, 0, 0),
                    normalIndex > 0 ? normals[normalIndex - 1] : glm::vec3(0, 0, 0),
                    uvIndex > 0 ? texCoords[uvIndex - 1] : glm::vec2(0, 0)
                );
            }
        }
    }

    file.close();

    // Creazione della mesh
    makeCurrent();
    auto m = std::make_unique<MeshCore>(std::move(ver));
    m->SetMaterial(mat);

    RendererDebugger::checkOpenGLError("model loading");
    setFocus();
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