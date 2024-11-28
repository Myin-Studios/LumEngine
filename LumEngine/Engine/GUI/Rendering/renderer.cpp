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
    RendererDebugger::checkOpenGLError("after initializeOpenGLFunctions");

	glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        exit(EXIT_FAILURE);
    }

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
    setupSkysphere();
}

void RendererCore::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	RendererDebugger::checkOpenGLError("resizing FBO");

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    update();
}

void RendererCore::paintGL()
{
    GLint previousFBO;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFBO);
    qDebug() << "Previous FBO binding:" << previousFBO;

    glClearColor(0.1, 0.1, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::cout << "FBO: " << FBO << "\nRBO: " << RBO << "\nFBO Texture: " << fboTexture << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        qDebug() << "Error unbinding FBO:" << error;
    }

    // Verifica che il FBO sia ancora valido
    // GLboolean isValidFBO = glIsFramebuffer(FBO);
    // qDebug() << "Is FBO valid:" << isValidFBO;
    // 
	// std::cout << "FBO: " << FBO << " RBO: " << RBO << " FBO Texture: " << fboTexture << std::endl;
    // 
    // // Enabling framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    // 
	// RendererDebugger::checkOpenGLError("FBO binding - rendering phase");
    // 
    // glClearColor(0.1, 0.1, 0.3, 1.0);
    // 
    // RendererDebugger::checkOpenGLError("glClearColor");
    // 
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 
    // RendererDebugger::checkOpenGLError("glClear");
    // 
    // glEnable(GL_DEPTH_TEST);
    // 
    // RendererDebugger::checkOpenGLError("enabling depth test");
    // 
    // glEnable(GL_CULL_FACE);
    // 
    // RendererDebugger::checkOpenGLError("enabling face culling");
    // 
    // glCullFace(GL_BACK);
    // 
    // RendererDebugger::checkOpenGLError("back face culling function");

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
    
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    
    if (canUpdateCamera)
    {
        UpdateCamera();
    }

    // glm::mat4 view = glm::lookAt(
    //     glm::vec3(
    //         editorCamera->GetTransform()->GetPosition().x(),
    //         editorCamera->GetTransform()->GetPosition().y(),
    //         editorCamera->GetTransform()->GetPosition().z()),
    //     glm::vec3(
    //         editorCamera->GetTransform()->GetPosition().x() + editorCamera->GetTransform()->forward.x(),
    //         editorCamera->GetTransform()->GetPosition().y() + editorCamera->GetTransform()->forward.y(),
    //         editorCamera->GetTransform()->GetPosition().z() + editorCamera->GetTransform()->forward.z()),
    //     glm::vec3(
    //         editorCamera->GetTransform()->up.x(),
    //         editorCamera->GetTransform()->up.y(),
    //         editorCamera->GetTransform()->up.z())
    // );
    // 
    // glm::mat4 viewTransposed = glm::transpose(view);
    // 
    // LumEngine::Physics::RayCast::SetViewMatrix(Mat4Core(
    //     viewTransposed[0][0], viewTransposed[0][1], viewTransposed[0][2], viewTransposed[0][3],
    //     viewTransposed[1][0], viewTransposed[1][1], viewTransposed[1][2], viewTransposed[1][3],
    //     viewTransposed[2][0], viewTransposed[2][1], viewTransposed[2][2], viewTransposed[2][3],
    //     viewTransposed[3][0], viewTransposed[3][1], viewTransposed[3][2], viewTransposed[3][3]
    // ));
    // 
    // glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)this->width() / (float)this->height(), 0.1f, 100.0f);
    // glm::mat4 projTransposed = glm::transpose(proj);
    // 
    // LumEngine::Physics::RayCast::SetProjectionMatrix(Mat4Core(
    //     projTransposed[0][0], projTransposed[0][1], projTransposed[0][2], projTransposed[0][3],
    //     projTransposed[1][0], projTransposed[1][1], projTransposed[1][2], projTransposed[1][3],
    //     projTransposed[2][0], projTransposed[2][1], projTransposed[2][2], projTransposed[2][3],
    //     projTransposed[3][0], projTransposed[3][1], projTransposed[3][2], projTransposed[3][3]
    // ));
    // 
    // if (!entities.empty())
    // {
    //     for (auto& e : entities)
    //     {
    //         if (!IsRunning())
    //         {
    //             e->DeserializeProperties();
    //         }
    //         else updateTimer->start();
    // 
    //         if (e->GetCoreProperty<MeshCore>() != nullptr)
    //         {
    //             e->GetCoreProperty<MeshCore>()->Draw();
    // 
    //             if (e->GetCoreProperty<Transform3DCore>() != nullptr)
    //             {
    //                 glm::mat4 tMat = glm::mat4(1.0f); // Inizia con una matrice identità
    //                 tMat = glm::translate(tMat, glm::vec3(
    //                     e->GetCoreProperty<Transform3DCore>()->position->x(),
    //                     e->GetCoreProperty<Transform3DCore>()->position->y(),
    //                     e->GetCoreProperty<Transform3DCore>()->position->z()
    //                 ));
    //                 tMat = glm::scale(tMat, glm::vec3(
    //                     e->GetCoreProperty<Transform3DCore>()->scale.x(),
    //                     e->GetCoreProperty<Transform3DCore>()->scale.y(),
    //                     e->GetCoreProperty<Transform3DCore>()->scale.z()
    //                 ));
    // 
    //                 e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("model", &tMat[0][0]);
    //             }
    //             else
    //             {
    //                 e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("model", &glm::mat4(1.0)[0][0]);
    //             }
    // 
    //             e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("view", &view[0][0]);
    // 
    //             e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setMat4x4("projection", &proj[0][0]);
    // 
    //             vector<glm::vec3> lightPositions;
    //             vector<glm::vec3> lightColors;
    //             std::vector<float> intensities;
    //             for (const auto& light : lights) {
    //                 lightPositions.emplace_back(light.GetTransform()->GetPosition().x(),
    //                     light.GetTransform()->GetPosition().y(),
    //                     light.GetTransform()->GetPosition().z());
    // 
    //                 lightColors.emplace_back(glm::vec3(
    //                     light.color.r(),
    //                     light.color.g(),
    //                     light.color.b()
    //                 ));
    // 
    //                 intensities.push_back(light.GetIntensity());
    //             }
    // 
    //             e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setVec3Array("lightPositions", lightPositions.size(), glm::value_ptr(lightPositions[0]));
    //             e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setVec3Array("lightColors", lightColors.size(), glm::value_ptr(lightColors[0]));
    //             e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setFloatArray("lightIntensities", intensities.size(), intensities.data());
    //             e->GetCoreProperty<MeshCore>()->GetMaterial()->GetShader()->setVec3("camPos", glm::vec3(
    //                 editorCamera->GetTransform()->GetPosition().x(),
    //                 editorCamera->GetTransform()->GetPosition().y(),
    //                 editorCamera->GetTransform()->GetPosition().z()
    //             ));
    // 
    //             if (auto pbrMat = std::dynamic_pointer_cast<PBR>(e->GetCoreProperty<MeshCore>()->GetMaterial())) {
    //                 Color::Color c(1.0f, 0.5f, 0.0f);
    //                 pbrMat->SetAlbedo(c);
    //                 pbrMat->SetMetallic(1.0f);
    //                 pbrMat->SetRoughness(0.4f);
    //             }
    //         }
    //     }
    // }
    // 
    // RendererDebugger::checkOpenGLError("after model loading");
    // 
	// // Disabling framebuffer
    // glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
    // glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    // 
	// fboShader->use();
	// glBindVertexArray(screenVAO);
    // glDisable(GL_DEPTH_TEST);
	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, fboTexture);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
	// glBindVertexArray(0);
    // 
	// RendererDebugger::checkOpenGLError("after fbo rendering");
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
    qDebug() << "Generated FBO:" << FBO;

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    
	RendererDebugger::checkOpenGLError("setting up FBO");

    // Create and attach texture
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

	RendererDebugger::checkOpenGLError("setting up FBO texture");

    // Create and attach RBO
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width(), height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	RendererDebugger::checkOpenGLError("setting up RBO");

    // Check FBO status
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    qDebug() << "FBO Status:" << status;
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        qDebug() << "Framebuffer is not complete!";
    }

    // Unbind everything
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &screenVBO);

    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);

    if (!screenQuad.empty())
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuad), &screenQuad[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

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
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Unable to open the file:" << path;
        return std::unique_ptr<MeshCore>();
    }

    QTextStream in(&file);

    vector<Vertex> ver;

    vector<glm::vec3> vertices;
    vector<glm::vec2> texCoords;
    vector<glm::vec3> normals;
    vector< unsigned int> indices;

    glm::vec3 pos = glm::vec3(0, 0, 0);
    glm::vec3 nor = glm::vec3(0, 0, 0);
    glm::vec2 uvs = glm::vec2(0, 0);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList tokens = line.split(" ");

        if (tokens.isEmpty()) continue;

        if (tokens[0] == "v") {
            float x = tokens[1].toFloat();
            float y = tokens[2].toFloat();
            float z = tokens[3].toFloat();

            pos = glm::vec3(x, y, z);
            vertices.push_back(pos);

        } else if (tokens[0] == "vt") {
            float u = tokens[1].toFloat();
            float v = tokens[2].toFloat();

            uvs = glm::vec2(u, v);
            texCoords.push_back(uvs);

        } else if (tokens[0] == "vn") {
            float nx = tokens[1].toFloat();
            float ny = tokens[2].toFloat();
            float nz = tokens[3].toFloat();

            nor = glm::vec3(nx, ny, nz);
            normals.push_back(nor);

        } else if (tokens[0] == "f") {
            for (int i = 1; i <= 3; i++) {
                QStringList vertexData = tokens[i].split("/");

                int vertexIndex = vertexData[0].toInt() - 1;
                int uvIndex = vertexData.size() > 1 ? vertexData[1].toInt() - 1 : -1;
                int normalIndex = vertexData.size() > 2 ? vertexData[2].toInt() - 1 : -1;

                ver.emplace_back(vertexIndex > -1 ? vertices[vertexIndex] : glm::vec3(0, 0, 0),
                                     normalIndex > -1 ? normals[normalIndex] : glm::vec3(0, 0, 0),
                                     uvIndex > -1 ? texCoords[uvIndex] : glm::vec2(0, 0));
            }
        }
    }

    file.close();

    makeCurrent();

    std::unique_ptr<MeshCore> m = std::make_unique<MeshCore>(ver);

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