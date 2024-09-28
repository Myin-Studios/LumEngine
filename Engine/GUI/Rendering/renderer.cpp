#include "renderer.h"

Renderer::Renderer(QWidget *parent)
        : QOpenGLWidget(parent)
{
    setAcceptDrops(true);
}

Renderer::~Renderer()
{
    cleanup();
}

void Renderer::initializeGL()
{
    initializeOpenGLFunctions();

    GLenum err = glewInit();
    if (GLEW_OK != err) {
        qCritical() << "GLEW Error: " << glewGetErrorString(err);
        return;
    }

    s = new Shader("../Engine/Resources/Shaders/baseVert.glsl", "../Engine/Resources/Shaders/baseFrag.glsl");

    setupFrameBuffer();
}

void Renderer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Renderer::paintGL()
{
    makeCurrent();

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClearColor(0.1, 0.1, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    if (s != nullptr)
    {
        s->use();
        s->setMat4x4("model", &glm::mat4(1.0f)[0][0]);
        s->setMat4x4("view", &glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))[0][0]);
        s->setMat4x4("projection", &glm::perspective(glm::radians(45.0f), (float)this->width() / (float)this->height(), 0.1f, 100.0f)[0][0]);

        if (!models.empty())
        {
            for (Mesh& m : models)
            {
                m.Draw(*s);
            }
        }
    }

    fboShader->use();
    glDisable(GL_DEPTH_TEST);

    glBindVertexArray(screenVAO);
    glDisable(GL_DEPTH_TEST);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    RendererDebugger::checkOpenGLError("rendering");
}

void Renderer::checkFrameBufferError()
{
    switch (glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
        case !GL_FRAMEBUFFER_COMPLETE:
        {
            cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
        }
        default:
        {
            return;
        }
    }
}

void Renderer::setupFrameBuffer()
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    checkFrameBufferError();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDeleteFramebuffers(1, &FBO);

    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &screenVBO);

    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);

    if (!screenQuad.empty())
        glBufferData(GL_ARRAY_BUFFER, screenQuad.size(), &screenQuad[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    glBindVertexArray(0);

    RendererDebugger::checkOpenGLError("setting up screen buffers");

    fboShader = new Shader("../Engine/Resources/Shaders/fboVertex.glsl", "../Engine/Resources/Shaders/fboFragment.glsl");
}

void Renderer::cleanup()
{
    makeCurrent();

    if (s)
    {
        delete s;
        s = nullptr;
    }

    doneCurrent();
}

void Renderer::dragEnterEvent(QDragEnterEvent* event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
    }
}

void Renderer::dropEvent(QDropEvent *event) {
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

void Renderer::loadModel(const QString& path) {
    if (path.endsWith(".obj"))
    {
        if (loadOBJ(path)) cout << "Model loaded successfully!" << endl;
    }
}

bool Renderer::loadOBJ(const QString& path) {
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qCritical() << "Unable to open the file:" << path;
        return false;
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

                ver.push_back(Vertex(vertexIndex > -1 ? vertices[vertexIndex] : glm::vec3(0, 0, 0), glm::vec3(), glm::vec2())
//                                     normalIndex > -1 ? normals[normalIndex] : glm::vec3(0, 0, 0),
//                                     uvIndex > -1 ? texCoords[uvIndex] : glm::vec2(0, 0))
                                     );

                indices.push_back(vertexIndex);
            }
        }
    }

    file.close();

    makeCurrent();

    models.emplace_back(ver);

    RendererDebugger::checkOpenGLError("model loading");

    update();

    return true;
}