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

#include "../LumTypes/Rendering/Lighting/Lighting.h"
#include "Camera/camera.h"
#include "../LumTypes/Entities/Entity.h"
#include "../LumTypes/Physics/Collisions.h"
#include "Meshes/Mesh.h"
#include "../LumEngine/Engine/Core/scriptingThreading.h"
#include "../LumEngine/Engine/Core/LumEngineAPI.h"

#include "GL/glew.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <vector>
#include <set>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <iostream>

using namespace std;

class RendererCore : public QOpenGLWidget
{
Q_OBJECT

public:
    ~RendererCore() override;

    static void RegisterInstance(RendererCore* instance)
    {
        s_instance = instance;
    }

    static RendererCore* GetInstance(QWidget* parent = nullptr) {
        if (s_instance == nullptr) {
            s_instance = new RendererCore(parent);
        }
        return s_instance;
    }

    vector<shared_ptr<BaseEntity>> GetEntities()
    {
        return this->entities;
    }

    void SetRunningThread(std::shared_ptr<ScriptRunnerThread> rt)
    {
        this->runningThread = std::move(rt);
    }

    bool IsRunning() const
    {
        return this->runningThread && this->runningThread->IsRunning();
    }

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

private:
    explicit RendererCore(QWidget* parent = nullptr);
    static RendererCore* s_instance;

    void UpdateCamera();

    void setupSkysphere();
    void checkFrameBufferError();
    void setupFrameBuffer();
    void cleanup();
    void loadModel(const QString& path);
    std::unique_ptr<MeshCore> loadOBJ(const QString& path, std::shared_ptr<Material> mat);

    Camera* editorCamera;
    QPoint mousePos;
    bool canUpdateCamera = false;
    set<int> keyPressed;

    QTimer* updateTimer;

    std::shared_ptr<MeshCore> skysphere;
    vector<shared_ptr<MeshCore>> models;
    vector<shared_ptr<BaseEntity>> entities;

    vector<Light> lights;

    GLuint FBO = 0;
    GLuint fboTexture = 0;
    GLuint depthTexture = 0;
    GLuint RBO = 0;
    GLuint screenVAO = 0;
    GLuint screenVBO = 0;
    const std::vector<GLfloat> screenQuad = {
        // positions   // texCoords
        -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
         1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
         1.0f,  1.0f, 1.0f, 1.0f, // Top-right

        -1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
         1.0f,  1.0f, 1.0f, 1.0f, // Top-right
        -1.0f,  1.0f, 0.0f, 1.0f  // Top-left
    };
    Shader* fboShader = nullptr;

    std::shared_ptr<ScriptRunnerThread> runningThread = nullptr;
};