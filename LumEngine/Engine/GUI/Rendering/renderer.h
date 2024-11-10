#pragma once

#include "Lighting/lighting.h"
#include "Camera/camera.h"

#include "GL/glew.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QtOpenGL/qopenglshaderprogram.h>
#include <vector>
#include <set>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <iostream>

#include "mesh.h"

using namespace std;

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT

public:
    explicit Renderer(QWidget *parent = nullptr);
    ~Renderer() override;

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
    
    void UpdateCamera();

    void setupSkysphere();
    void checkFrameBufferError();
    void setupFrameBuffer();
    void cleanup();
    void loadModel(const QString& path);
    std::shared_ptr<Mesh> loadOBJ(const QString& path, std::shared_ptr<Material> mat);

    Camera* editorCamera;
    QPoint mousePos;
    bool canUpdateCamera = false;
    set<int> keyPressed;

    QTimer* updateTimer;

    std::shared_ptr<Mesh> skysphere;
    vector<shared_ptr<Mesh>> models;

    vector<Light> lights;

    unsigned int FBO;
    unsigned int fboTexture;
    unsigned int RBO;
    unsigned int screenVAO;
    unsigned int screenVBO;
    vector<GLfloat> screenQuad = {
            // Coords    // texCoords
            -1.0f, -1.0f, 0.0f, 0.0f, // Top-left
            1.0f, -1.0f, 1.0f, 0.0f, // Top-right
            1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right

            -1.0f, 1.0f, 0.0f, 1.0f, // Bottom-left
            -1.0f, -1.0f, 0.0f, 0.0f, // Top-left
            1.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
    };
    Shader* fboShader = nullptr;
};