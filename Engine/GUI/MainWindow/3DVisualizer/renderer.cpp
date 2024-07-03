/////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                                                                   ///
///                                      THIS CODE IS PART OF:                                        ///
///                                       CryoMoon Engine (C)                                         ///
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

Renderer::Renderer(QWidget *parent)
        : QOpenGLWidget(parent),
          m_program(nullptr)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
}

Renderer::~Renderer()
{
    makeCurrent();
    m_vbo.destroy();
    m_vao.destroy();
    delete m_program;
    doneCurrent();
}

void Renderer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        m_mouseRightPressed = true;
//        QCursor::setPos(mapToGlobal(rect().center()));
        lastPos = event->pos();
//        QCursor cur(Qt::BlankCursor);
//        QApplication::setOverrideCursor(cur);
//        QApplication::changeOverrideCursor(cur);

        setFocus();
    }
}

void Renderer::mouseReleaseEvent(QMouseEvent *event)
{
    m_mouseRightPressed = false;
    m_KeyPressed.clear();
    QCursor cur(Qt::ArrowCursor);
    QApplication::setOverrideCursor(cur);
    QApplication::changeOverrideCursor(cur);
    nonZeroDeltaX = 0.0f;
    nonZeroDeltaY = 0.0f;
    clearFocus();
}

void Renderer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_mouseRightPressed) {

        QPoint currentPos = event->pos();

        float deltaX = currentPos.x() - lastPos.x();
        float deltaY = currentPos.y() - lastPos.y();

        float sensitivity = 0.1f;

        m_camera.rotate(deltaX * sensitivity, -deltaY * sensitivity, 0.0f);

        lastPos = currentPos;

        update();
    }
}

void Renderer::keyPressEvent(QKeyEvent *event)
{
    m_KeyPressed.insert(event->key());

    QOpenGLWidget::keyPressEvent(event);
    update();
}

void Renderer::keyReleaseEvent(QKeyEvent *event)
{
    m_KeyPressed.remove(event->key());

    QOpenGLWidget::keyReleaseEvent(event);
    update();
}

QVector2D Renderer::smoothMouseMovement(float deltaX, float deltaY)
{
    QVector2D currentDelta(deltaX, deltaY);

    if (smoothDeltas.size() >= 5)
        smoothDeltas.erase(smoothDeltas.begin());

    smoothDeltas.push_back(currentDelta);

    QVector2D averageDelta(0.0f, 0.0f);

    for (const QVector2D delta : smoothDeltas)
    {
        averageDelta += delta;
    }

    averageDelta /= static_cast<float>(smoothDeltas.size());

    return averageDelta;
}

void Renderer::updateCameraPos()
{
    if (m_KeyPressed.contains(Qt::Key_W))
    {
        float zoomSpeed = 0.1f;
        m_camera.move(m_camera.front() * zoomSpeed);
    }
    if (m_KeyPressed.contains(Qt::Key_S))
    {
        float zoomSpeed = 0.1f;
        m_camera.move(-m_camera.front() * zoomSpeed);
    }
    if (m_KeyPressed.contains(Qt::Key_D))
    {
        float zoomSpeed = 0.1f;
        m_camera.move(m_camera.right() * zoomSpeed);
    }
    if (m_KeyPressed.contains(Qt::Key_A))
    {
        float zoomSpeed = 0.1f;
        m_camera.move(-m_camera.right() * zoomSpeed);
    }
    if (m_KeyPressed.contains(Qt::Key_E))
    {
        float zoomSpeed = 0.1f;
        m_camera.move(m_camera.up() * zoomSpeed);
    }
    if (m_KeyPressed.contains(Qt::Key_Q))
    {
        float zoomSpeed = 0.1f;
        m_camera.move(-m_camera.up() * zoomSpeed);
    }

    if (QCursor::pos().x() >= mapToGlobal(rect().center()).x() + width() / 2)
    {
        m_mouseRightPressed = false;
        QCursor::setPos(mapToGlobal(rect().center()).x() - width() / 2, QCursor::pos().y());
        lastPos = QPoint(mapToGlobal(rect().center()).x() - width() / 2, lastPos.y());
        m_mouseRightPressed = true;
    }
    else if (QCursor::pos().x() <= mapToGlobal(rect().center()).x() - width() / 2)
    {
        m_mouseRightPressed = false;
        QCursor::setPos(mapToGlobal(rect().center()).x() + width() / 2, QCursor::pos().y());
        lastPos = QPoint(mapToGlobal(rect().center()).x() + width() / 2, lastPos.y());
        m_mouseRightPressed = true;
    }
    if (QCursor::pos().y() >= mapToGlobal(rect().center()).y() + height() / 2)
    {
        m_mouseRightPressed = false;
        QCursor::setPos(QCursor::pos().x(), mapToGlobal(rect().center()).y() - height() / 2);
        lastPos = QPoint(lastPos.x(), QCursor::pos().y());
        m_mouseRightPressed = true;
    }
    else if (QCursor::pos().y() <= mapToGlobal(rect().center()).y() - height() / 2)
    {
        m_mouseRightPressed = false;
        QCursor::setPos(QCursor::pos().x(), mapToGlobal(rect().center()).y() + height() / 2);
        lastPos = QPoint(lastPos.x(), QCursor::pos().y());
        m_mouseRightPressed = true;
    }
}

void Renderer::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    initShaders();

    m_camera.setPosition(QVector3D(0.0f, 0.0f, 3.0f));
    m_camera.setRotation(-90.0f, 0.0f, 0.0f);

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.create();
    m_vbo.bind();

    m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(GLfloat));

    ebo = QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    ebo.create();
    ebo.bind();
    ebo.allocate(indices.data(), indices.size() * sizeof(GLuint));

    setupVertexAttribs();
    initShadowMap();

    m_vbo.release();
    m_program->release();
}

void Renderer::initShaders()
{
    m_program = new QOpenGLShaderProgram();
    shadowShader = new QOpenGLShaderProgram();

    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "../../Engine/Shaders/vertex_shader.glsl")) {
        qDebug() << "Vertex Shader Error:" << m_program->log();
    }
    if (!m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "../../Engine/Shaders/fragment_shader.glsl")) {
        qDebug() << "Fragment Shader Error:" << m_program->log();
    }
    if (!m_program->link()) {
        qDebug() << "Shader Program Link Error:" << m_program->log();
    }
    if (!m_program->bind()) {
        qDebug() << "Shader Program Bind Error:" << m_program->log();
    }

    if (!shadowShader->addShaderFromSourceFile(QOpenGLShader::Vertex, "../../Engine/Shaders/shadow_vertex_shader.glsl")) {
        qDebug() << "Shadow Vertex Shader Error:" << shadowShader->log();
    }
    if (!shadowShader->addShaderFromSourceFile(QOpenGLShader::Fragment, "../../Engine/Shaders/shadow_fragment_shader.glsl")) {
        qDebug() << "Shadow Fragment Shader Error:" << shadowShader->log();
    }
    if (!shadowShader->link()) {
        qDebug() << "Shadow Shader Program Link Error:" << shadowShader->log();
    }
    if (!shadowShader->bind()) {
        qDebug() << "Shadow Shader Program Bind Error:" << shadowShader->log();
    }
}

void Renderer::initShadowMap()
{
    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void Renderer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Renderer::updateTimeOfDay()
{
    // Incrementa il tempo del giorno
    timeOfDay += 0.001f;
    if (timeOfDay > 1.0f)
        timeOfDay = 0.0f;

    // Aggiorna la posizione della luce solare
    sunPosition = QVector3D(sin(timeOfDay * 2.0 * M_PI) * 50.0,
                            cos(timeOfDay * 2.0 * M_PI) * 50.0,
                            0.0);

    update();
}

void Renderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shadowPass();

    // Render of main scene
    m_program->bind();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    QMatrix4x4 model;
    model.translate(_x, _y, _z);
    m_program->setUniformValue("model", model);

    QMatrix4x4 view = m_camera.getViewMatrix();
    m_program->setUniformValue("view", view);

    QMatrix4x4 projection;
    projection.perspective(45.0f, float(width()) / float(height()), 0.1f, 100.0f);
    m_program->setUniformValue("projection", projection);

    m_program->setUniformValue("viewPos", m_camera.position());

    m_program->setUniformValue("roughness", 1.0f);
    m_program->setUniformValue("lightPos", sunPosition);
    m_program->setUniformValue("lightColor", QVector3D(1.0f, 1.0f, 1.0f));
    m_program->setUniformValue("objectColor", QVector3D(_r, _g, _b));

    m_program->setUniformValue("ambientStrength", 0.1f);
    m_program->setUniformValue("specularStrength", 0.5f);
    m_program->setUniformValue("shininess", 32.0f);
    m_program->setUniformValue("scatteringFactor", 0.2f);


    // Passa la shadow map
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    m_program->setUniformValue("shadowMap", 1);

    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    renderScene(*m_program);

    ebo.bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    if (m_mouseRightPressed)
    {
        updateTimeOfDay();
        updateCameraPos();
    }

    m_program->release();
}


void Renderer::shadowPass()
{
    glViewport(0, 0, 1920, 1080);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    shadowShader->bind();
    shadowShader->setUniformValue("lightSpaceMatrix", lightSpaceMatrix);
    renderScene(*shadowShader);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::renderScene(QOpenGLShaderProgram &shader)
{
    QMatrix4x4 model;

    // Renderizza il cubo
    model.translate(QVector3D(0.0f, 0.5f, 0.0f));
    shader.setUniformValue("model", model);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size());

    // Renderizza il piano
    model.setToIdentity();
    shader.setUniformValue("model", model);
    glDrawArrays(GL_TRIANGLES, 0, planeVertices.size());
}

void Renderer::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void Renderer::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        if (urlList.size() > 0)
        {
            QString fileName = urlList.at(0).toLocalFile();
            if (fileName.contains(".gltf"))
                loadGLTF(fileName);
            if (fileName.contains(".obj"))
                loadOBJ(fileName);

            setScale(0.0000000000001f);
        }
    }
}

void Renderer::setupVertexAttribs()
{
    m_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    m_vbo.release();
}

void Renderer::updateVertices()
{
    makeCurrent();

    if (vertices.empty()) {
        qWarning() << "No vertex to update";
        return;
    }

    m_vbo.bind();
    m_vbo.allocate(vertices.data(), vertices.size() * sizeof(float));

    setupVertexAttribs(); // Configura gli attributi dei vertici

    m_vbo.release();

    // Print vertices values
//    qDebug() << "Updated vertices:";
//    for (size_t i = 0; i < vertices.size(); i += 3) {
//        qDebug() << "Vertex" << i / 3 << ":" << vertices[i] << vertices[i + 1] << vertices[i + 2];
//    }

    update();
}

void Renderer::loadGLTF(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open the file" << filePath;
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument document = QJsonDocument::fromJson(data);
    if (document.isNull() || !document.isObject()) {
        qWarning() << "Invalid JSON file" << filePath;
        return;
    }

    QJsonObject gltf = document.object();

    if (!gltf.contains("buffers") || !gltf["buffers"].isArray()) {
        qWarning() << "No buffers found in the file" << filePath;
        return;
    }
    QJsonArray buffers = gltf["buffers"].toArray();

    if (!gltf.contains("bufferViews") || !gltf["bufferViews"].isArray()) {
        qWarning() << "No bufferViews found in the file" << filePath;
        return;
    }
    QJsonArray bufferViews = gltf["bufferViews"].toArray();

    if (!gltf.contains("accessors") || !gltf["accessors"].isArray()) {
        qWarning() << "No accessors found in the file" << filePath;
        return;
    }
    QJsonArray accessors = gltf["accessors"].toArray();

    if (gltf.contains("meshes") && gltf["meshes"].isArray()) {
        QJsonArray meshes = gltf["meshes"].toArray();
        if (meshes.isEmpty()) {
            qWarning() << "No meshes found in the file" << filePath;
            return;
        }

        for (const QJsonValue &meshValue : meshes) {
            if (meshValue.isObject()) {
                QJsonObject meshObject = meshValue.toObject();
                QJsonArray primitives = meshObject["primitives"].toArray();
                if (primitives.isEmpty()) {
                    qWarning() << "No primitives found in the mesh" << filePath;
                    return;
                }

                for (const QJsonValue &primitiveValue : primitives) {
                    if (primitiveValue.isObject()) {
                        QJsonObject primitiveObject = primitiveValue.toObject();
                        QJsonObject attributes = primitiveObject["attributes"].toObject();

                        // Caricamento delle posizioni
                        if (!attributes.contains("POSITION")) {
                            qWarning() << "Attribute POSITION not found in primitives" << filePath;
                            return;
                        }

                        int positionAccessorIndex = attributes["POSITION"].toInt();
                        QJsonObject positionAccessor = accessors[positionAccessorIndex].toObject();
                        int positionBufferViewIndex = positionAccessor["bufferView"].toInt();
                        QJsonObject positionBufferView = bufferViews[positionBufferViewIndex].toObject();
                        int positionByteOffset = positionBufferView["byteOffset"].toInt();
                        int positionByteLength = positionBufferView["byteLength"].toInt();
                        QByteArray bufferData = data.mid(positionByteOffset, positionByteLength);
                        const float *positions = reinterpret_cast<const float*>(bufferData.constData());
                        int positionCount = positionAccessor["count"].toInt();

                        const float *normals = nullptr;
                        if (attributes.contains("NORMAL")) {
                            int normalAccessorIndex = attributes["NORMAL"].toInt();
                            QJsonObject normalAccessor = accessors[normalAccessorIndex].toObject();
                            int normalBufferViewIndex = normalAccessor["bufferView"].toInt();
                            QJsonObject normalBufferView = bufferViews[normalBufferViewIndex].toObject();
                            int normalByteOffset = normalBufferView["byteOffset"].toInt();
                            int normalByteLength = normalBufferView["byteLength"].toInt();
                            bufferData = data.mid(normalByteOffset, normalByteLength);
                            normals = reinterpret_cast<const float*>(bufferData.constData());
                        } else {
                            qWarning() << "Attribute NORMAL not found" << filePath;
                        }

                        // Popola il vettore dei vertici e delle normali
                        vertices.clear();
                        for (int i = 0; i < positionCount; ++i) {
                            vertices.push_back(positions[i * 3]);
                            vertices.push_back(positions[i * 3 + 1]);
                            vertices.push_back(positions[i * 3 + 2]);

                            qDebug() << "Position[" << i << "]: (" << positions[i * 3] << ", " << positions[i * 3 + 1] << ", " << positions[i * 3 + 2] << ")";

                            if (normals) {
                                vertices.push_back(normals[i * 3]);
                                vertices.push_back(normals[i * 3 + 1]);
                                vertices.push_back(normals[i * 3 + 2]);

                                qDebug() << "Normal[" << i << "]: (" << normals[i * 3] << ", " << normals[i * 3 + 1] << ", " << normals[i * 3 + 2] << ")";
                            } else {
                                qDebug() << "There aren't normals.";
                                // Se non ci sono normali, aggiungi valori di default
                                vertices.push_back(0.0f);
                                vertices.push_back(0.0f);
                                vertices.push_back(0.0f);
                            }
                        }

                        updateVertices(); // Aggiorna il VBO con le nuove posizioni e normali
                    }
                }
            }
        }
    } else {
        qWarning() << "No meshes found in the file" << filePath;
    }
}

void Renderer::loadOBJ(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Unable to open the file" << filePath;
        return;
    }

    QString line;
    vertices.clear();

    do
    {
        line = file.readLine();

        if (line.contains("v "))
        {
            QStringList vData = line.split(" ");
            vData.removeAll("v");

            vertices.push_back(vData[0].toFloat());
            vertices.push_back(vData[1].toFloat());
            vertices.push_back(vData[2].toFloat());
        }

        if (line.contains("vn "))
        {
            QStringList nData = line.split(" ");
            nData.removeAll("vn");

            vertices.push_back(nData[0].toFloat());
            vertices.push_back(nData[1].toFloat());
            vertices.push_back(nData[2].toFloat());
        }

        if (line.contains("f "))
        {
            QStringList fData = line.split(" ");
            fData.removeAll("f");

            for (QString s : fData)
            {
                QStringList unpackedData = s.split("/");

                qDebug() << unpackedData;

                indices.push_back(unpackedData[0].toInt());
                indices.push_back(unpackedData[1].toInt());
                indices.push_back(unpackedData[2].toInt());
            }
        }
    } while (!line.isNull());

    do
    {
        line = file.readLine();


    } while (!line.isNull());

    updateVertices();
}