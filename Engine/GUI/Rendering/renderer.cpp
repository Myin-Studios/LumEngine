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
}

void Renderer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
}

void Renderer::paintGL()
{
    makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.3, 1.0);

    glEnable(GL_DEPTH_TEST);

//    if (!models.empty())
//    {
//        for (auto&& m: models)
//        {
//            m.Draw(*s);
//        }
//    }

    if (model != nullptr)
        model->Draw(*s);

    glDisable(GL_DEPTH_TEST);

    RendererDebugger::checkOpenGLError("rendering");
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

                ver.push_back(Vertex(vertexIndex > -1 ? vertices[vertexIndex] : glm::vec3(0, 0, 0),
                                     normalIndex > -1 ? normals[normalIndex] : glm::vec3(0, 0, 0),
                                     uvIndex > -1 ? texCoords[uvIndex] : glm::vec2(0, 0)));

                indices.push_back(vertexIndex);
            }
        }
    }

    file.close();

//    for (auto& v : ver)
//    {
//        cout << "Pos: (" << v.Position.x << ", " << v.Position.y << ", " << v.Position.z << ")" << endl;
//    }

    s = new Shader("../Engine/Resources/Shaders/baseVert.glsl", "../Engine/Resources/Shaders/baseFrag.glsl");

    model = new Mesh(ver);
    models.emplace_back(ver);

    RendererDebugger::checkOpenGLError("model loading");

    update();

    return true;
}