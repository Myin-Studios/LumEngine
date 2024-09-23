#pragma once

#include "GL/glew.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QtOpenGL/qopenglshaderprogram.h>
#include <vector>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QFile>
#include <QTextStream>
#include <QDebug>
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

private:
    void cleanup();
    void loadModel(const QString& path);
    bool loadOBJ(const QString& path);

    Mesh* model = nullptr;
    vector<Mesh> models;
    Shader* s = nullptr;
};