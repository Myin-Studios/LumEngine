#ifndef WINDOW_H
#define WINDOW_H

#define GLEW_USE_LIBRARIES
#include <GL/glew.h>
#include "camera.h"
#include <wx/wx.h>
#include <wx/glcanvas.h>
#include <wx/dnd.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <vector>
#include "GameBehaviour/CMTime.h"

#ifdef _WIN32
// Definire NOMINMAX per evitare conflitti con min e max di std::numeric_limits
#ifndef NOMINMAX
#define NOMINMAX
#endif

#define byte win_byte_override // Definisci un macro temporanea

#include <Windows.h>
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

// Rimuovi la definizione temporanea
#undef byte

void SetDarkModeForWindow(HWND hwnd) {
    if (hwnd == NULL) return;

    BOOL USE_DARK_MODE = true;
    BOOL SET_IMMERSIVE_DARK_MODE_SUCCESS = SUCCEEDED(DwmSetWindowAttribute(
            hwnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
            &USE_DARK_MODE, sizeof(USE_DARK_MODE)));
}
#endif

#define CHECK_GL_ERROR() {\
    GLenum err = glGetError();\
    if (err != GL_NO_ERROR) {\
        std::cerr << "OpenGL error: " << err << " at " << __FILE__ << ":" << __LINE__ << std::endl;\
    }\
}

using namespace std;

class FileLoader
{
public:
    FileLoader() = default;

    static string Scanner(const string& filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filePath << endl;
            return "";
        }

        return filePath;
    }

    static string Reader(const string& fileName)
    {
        string path = Scanner(fileName);

        if (!path.empty())
        {
            ifstream file(path);

            stringstream buffer;
            buffer << file.rdbuf();
            return buffer.str();
        }

        return "";
    }
};

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoord;
};

class Renderer : public wxGLCanvas {
public:
    Renderer(wxWindow* parent);
    ~Renderer();

    void modelImporter(const string& fileName);

private:
    void update();

    static int* getGLAttributes() {
        static int attributes[] = {
                WX_GL_RGBA,
                WX_GL_DOUBLEBUFFER,
                WX_GL_DEPTH_SIZE, 16,
                WX_GL_MAJOR_VERSION, 4,
                WX_GL_MINOR_VERSION, 6,
                WX_GL_CORE_PROFILE,
                0
        };
        return attributes;
    }

    void OnIdle(wxIdleEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBackground(wxEraseEvent& event);
    void OnMouseClick(wxMouseEvent& event);
    void OnKeyPressed(wxKeyEvent& event);
    void OnKeyReleased(wxKeyEvent& event);
    void OnFocus(wxFocusEvent& event);
    void OnKillFocus(wxFocusEvent& event);
    void LoadShader(string vertPath, string fragPath, GLuint& shaderProgram);
    GLuint ShaderCompiler(GLenum shaderType, const std::string& source);
    void SetupVertexData();
    GLuint generateSkyboxTexture();
    void renderSkybox();
    void renderModel(GLuint& vao, GLuint& VBO, GLuint& shaderProgram, const vector<Vertex>& mesh);
    void setupBuffers();

    wxGLContext* m_context;
    bool glewInitialized = false;
    bool isRightMouseClicked = false;
    bool isFocused = false;
    bool shouldExit = false;
    GLuint m_shaderProgram;
    GLuint m_skyboxShaderProgram;
    Camera m_cam;
    GLuint VAO, VBO;
    int width, height;

    glm::vec3 camVel = glm::vec3(0.0f);
    const float moveSpeed = .1f; // Velocità di movimento
    const float damping = 0.1f; // Fattore di frizione

    const int* attribs = new int[] {
            WX_GL_RGBA,
            WX_GL_DOUBLEBUFFER,
            WX_GL_DEPTH_SIZE, 16,
            WX_GL_MAJOR_VERSION, 4,
            WX_GL_MINOR_VERSION, 6,
            WX_GL_CORE_PROFILE,
            0
    };

    vector<vector<Vertex>> meshes;
    vector<GLuint> VAOs, _shaderPrograms;
    std::vector<GLint> firsts;
    std::vector<GLsizei> counts;

    vector<GLfloat> skyboxVertices = {
            // Front face
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            // Back face
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            // Left face
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,

            // Right face
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,

            // Top face
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,

            // Bottom face
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
    };

    vector<GLfloat> triangle = {
            0.5f, -0.5f, -0.0f,
            0.0f, 0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
    };
    GLuint skyboxTexture;
    GLuint skyboxVAO;
    GLuint skyboxVBO;

    set<int> keyPressedV;

wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(Renderer, wxGLCanvas)
                EVT_IDLE(Renderer::OnIdle)
                EVT_CLOSE(Renderer::OnClose)
                EVT_PAINT(Renderer::OnPaint)
                EVT_SIZE(Renderer::OnSize)
                EVT_ERASE_BACKGROUND(Renderer::OnEraseBackground)
                EVT_KEY_DOWN(Renderer::OnKeyPressed)
                EVT_KEY_UP(Renderer::OnKeyReleased)
                EVT_RIGHT_DOWN(Renderer::OnMouseClick)
                EVT_RIGHT_UP(Renderer::OnMouseClick)
                EVT_MOTION(Renderer::OnMouseClick)
                EVT_SET_FOCUS(Renderer::OnFocus)
                EVT_KILL_FOCUS(Renderer::OnKillFocus)
wxEND_EVENT_TABLE()

class DropTarget : public wxFileDropTarget {
public:
    DropTarget(Renderer* renderer) : m_renderer(renderer) {}

    virtual bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString& filenames) override {
        if (!filenames.empty())
        {
            for (auto &fn : filenames)
            {
                if (!fn.empty())
                {
                    if (fn.find(".obj"))
                    {
                        cout << "obj dropped!" << endl;
                        m_renderer->modelImporter(FileLoader::Scanner((string) fn));
                    }
                }
            }
            return true;
        }

        return false;
    }

private:
    Renderer* m_renderer;
};

Renderer::Renderer(wxWindow* parent)
        : wxGLCanvas(parent, wxID_ANY, getGLAttributes()),
          m_context(new wxGLContext(this)), glewInitialized(false) {
    // Set dark background for the canvas
    SetBackgroundStyle(wxBG_STYLE_PAINT);
    SetBackgroundColour(wxColour(45, 45, 48)); // Dark gray color

    // Initialize camera and other variables
    m_cam = Camera();

    // Initialize OpenGL context
    wxGLCanvas::SetCurrent(*m_context);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
        return;
    }
    glewInitialized = true;

    // OpenGL states
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    LoadShader("../../Engine/Shaders/base_vert.glsl", "../../Engine/Shaders/base_frag.glsl", m_shaderProgram);
    LoadShader("../../Engine/Shaders/base_vert.glsl", "../../Engine/Shaders/base_frag.glsl", m_skyboxShaderProgram);
    SetupVertexData();
    skyboxTexture = generateSkyboxTexture();
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    delete m_context;
}

void Renderer::update() {
    GameCore::update(); // it updates the deltaTime

    if (isRightMouseClicked)
    {
        glm::vec3 targetVel = glm::vec3(0.0f); // Reset target velocity

        // Update target velocity based on pressed keys
        if (keyPressedV.count('W') > 0) m_cam.move(m_cam.front() * moveSpeed);
        if (keyPressedV.count('S') > 0) m_cam.move(-m_cam.front() * moveSpeed);
        if (keyPressedV.count('A') > 0) m_cam.move(-m_cam.right() * moveSpeed);
        if (keyPressedV.count('D') > 0) m_cam.move(m_cam.right() * moveSpeed);
        if (keyPressedV.count('Q') > 0) m_cam.move(-m_cam.up() * moveSpeed / 3.0f);
        if (keyPressedV.count('E') > 0) m_cam.move(m_cam.up() * moveSpeed / 3.0f);
//
//        // Smoothly interpolate the camera velocity
//        camVel = glm::mix(camVel, targetVel, .1f); // Use a smoother factor here
//        m_cam.move(camVel);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    // Rendering Skybox
    renderSkybox();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Use the shader program
    glUseProgram(m_shaderProgram);

    // Set the uniform matrices and vectors
    GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(m_shaderProgram, "view");
    GLint projLoc = glGetUniformLocation(m_shaderProgram, "projection");
    GLint colLoc = glGetUniformLocation(m_shaderProgram, "col");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = m_cam.getViewMat();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 20000.0f);

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
    glUniform3fv(colLoc, 1, &glm::vec3(0.0f, 1.0f, 0.0f)[0]);


    CHECK_GL_ERROR();


    if (!VAOs.empty() && !firsts.empty() && !counts.empty())
    {
        if (!meshes.empty())
        {
            glBindVertexArray(VAOs.back());
            glDrawArrays(GL_TRIANGLES, 0, meshes.back().size());
            glBindVertexArray(0); // Unbind VAO
        }
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    SwapBuffers();
    Refresh();
}

void Renderer::OnIdle(wxIdleEvent& event) {
    if (shouldExit) {
        Close(true);
        return;
    }

    update();
    event.RequestMore();
}

void Renderer::OnClose(wxCloseEvent& event) {
    shouldExit = true;
    event.Skip();
}

void Renderer::OnPaint(wxPaintEvent& event) {
    if (!IsShown()) return;

    if (!glewInitialized) {
        wxGLCanvas::SetCurrent(*m_context);
        glewInitialized = (glewInit() == GLEW_OK);
    }

    update();
}

void Renderer::OnSize(wxSizeEvent& event) {
    int w, h;
    GetClientSize(&w, &h);
    width = w;
    height = h;

    wxGLCanvas::SetCurrent(*m_context);

    glViewport(0, 0, w, h);
}

void Renderer::OnEraseBackground(wxEraseEvent& event) {
    // Do nothing to prevent flickering
}

void Renderer::OnMouseClick(wxMouseEvent& event) {
    static int lastX = 0, lastY = 0;
    if (event.RightIsDown()) {
        isRightMouseClicked = true;

        int x, y;
        event.GetPosition(&x, &y);

        if (!isFocused) {
            lastX = x;
            lastY = y;
            isFocused = true;
        }

        int deltaX = x - lastX;
        int deltaY = y - lastY;

        float sensitivity = .5f; // Adjust sensitivity as needed
        m_cam.rotate(deltaX * sensitivity, -deltaY * sensitivity, 0.0f);

        lastX = x;
        lastY = y;
    } else {
        isRightMouseClicked = false;
        isFocused = false;
    }
}

void Renderer::OnKeyPressed(wxKeyEvent& event) {
    keyPressedV.insert(event.GetKeyCode());
}

void Renderer::OnKeyReleased(wxKeyEvent& event) {
    keyPressedV.erase(event.GetKeyCode());
}

void Renderer::OnFocus(wxFocusEvent& event) {
    isFocused = true;
}

void Renderer::OnKillFocus(wxFocusEvent& event) {
    isFocused = false;
}

void Renderer::LoadShader(string vertPath, string fragPath, GLuint& shaderProgram) {
    string vertexShaderSource = FileLoader::Reader(vertPath);
    string fragmentShaderSource = FileLoader::Reader(fragPath);

    GLuint vertexShader = ShaderCompiler(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = ShaderCompiler(GL_FRAGMENT_SHADER, fragmentShaderSource);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cerr << "Error linking shader program:\n" << infoLog << endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLuint Renderer::ShaderCompiler(GLenum shaderType, const string& source) {
    GLuint shader = glCreateShader(shaderType);
    const char* cstr = source.c_str();
    glShaderSource(shader, 1, &cstr, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cerr << "Error compiling shader:\n" << infoLog << endl;
    }

    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

    return shader;
}

void Renderer::SetupVertexData() {
    // Configurazione del VAO e VBO per lo skybox
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(GLfloat), skyboxVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindVertexArray(0);

    // Configurazione del VAO e VBO per il triangolo (o altri oggetti)
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, triangle.size() * sizeof(GLfloat), triangle.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    glBindVertexArray(0);
}

GLuint Renderer::generateSkyboxTexture() {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width = 512;
    int height = 512;
    std::vector<unsigned char> data(width * height * 3);

    for (int face = 0; face < 6; ++face) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = (y * width + x) * 3;

                // Gradient color
                data[index] = static_cast<unsigned char>((x / float(width)) * 255); // Red
                data[index + 1] = static_cast<unsigned char>((y / float(height)) * 255); // Green
                data[index + 2] = 128; // Blue (fixed value for simplicity)
            }
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.data());
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Renderer::renderSkybox() {
    glDepthFunc(GL_LEQUAL);
    glUseProgram(m_skyboxShaderProgram);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(glm::mat3(m_cam.getViewMat()));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

    GLint modelLoc = glGetUniformLocation(m_skyboxShaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(m_skyboxShaderProgram, "view");
    GLint projLoc = glGetUniformLocation(m_skyboxShaderProgram, "projection");
    GLint viewPosLoc = glGetUniformLocation(m_skyboxShaderProgram, "viewPos");

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
    glUniform3fv(viewPosLoc, 1, &m_cam.getPos()[0]);

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}

void Renderer::setupBuffers() {
    // Flatten the mesh data and setup individual VAOs
    std::vector<Vertex> allVertices;
    GLint startIndex = 0;

    // Generate VAOs
    VAOs.resize(meshes.size());
    glGenVertexArrays(meshes.size(), VAOs.data());

    // Create and bind the VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    for (size_t i = 0; i < meshes.size(); ++i) {
        const auto& mesh = meshes[i];

        // Accumulate vertices
        allVertices.insert(allVertices.end(), mesh.begin(), mesh.end());

        // Bind the VAO for this mesh
        glBindVertexArray(VAOs[i]);

        // Set vertex attribute pointers
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, Position) + startIndex * sizeof(Vertex)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, Normal) + startIndex * sizeof(Vertex)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(offsetof(Vertex, TexCoord) + startIndex * sizeof(Vertex)));

        firsts.push_back(startIndex);
        counts.push_back(mesh.size());
        startIndex += mesh.size();
    }

    // Upload the vertex data to the GPU after setting up all VAOs
    glBufferData(GL_ARRAY_BUFFER, allVertices.size() * sizeof(Vertex), allVertices.data(), GL_STATIC_DRAW);

    // Unbind VAO
    glBindVertexArray(0);
}


void Renderer::renderModel(GLuint &vao, GLuint &vbo, GLuint& shaderProgram, const vector<Vertex> &mesh) {

    cout << "Rendering model..." << endl;

    if (mesh.empty())
    {
        cout << "Sorry, the mesh is empty. I can't render it!" << endl;
        return;
    }

    // Pulizia del buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setupBuffers();

    // Carica e usa lo shader program
    LoadShader("../../Engine/Shaders/base_vert.glsl", "../../Engine/Shaders/frag.glsl", m_shaderProgram);
//    glUseProgram(m_shaderProgram);
//    cout << m_shaderProgram << endl;
//
//    // Set uniform matrices and vectors
//    GLint modelLoc = glGetUniformLocation(m_shaderProgram, "model");
//    GLint viewLoc = glGetUniformLocation(m_shaderProgram, "view");
//    GLint projLoc = glGetUniformLocation(m_shaderProgram, "projection");
//    GLint viewPosLoc = glGetUniformLocation(m_shaderProgram, "viewLoc");
//    glm::mat4 model = glm::mat4(1.0f);
//    glm::mat4 view = m_cam.getViewMat();
//    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
//
//    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
//    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
//    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
//
//    CHECK_GL_ERROR();
//
//    // Abilita depth test e face culling
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//
//    // Unbind VAO
//    glBindVertexArray(0);

    // Aggiornamento
    update();
}

void Renderer::modelImporter(const std::string &fileName) {
    ifstream file(FileLoader::Scanner(fileName));

    stringstream line;
    line << file.rdbuf();

    Vertex v{};
    vector<Vertex> mesh;

    vector<glm::vec3> positions;
    vector<glm::vec3> normals;
    vector<glm::vec2> texcoords;

    vector<glm::vec3> indices;

    bool beginPos = false, beginNor = false, beginTex = false, beginIdx = false, beginNam = false;

    bool hasTexCoordInfo = false;

    int vecPos = 0;

    string token;

    while (line >> token)
    {
        if (token == "v")
        {
            beginPos = true;
            beginNam = false;
            continue;
        }
        else if (token == "vn")
        {
            beginNor = true;
            beginPos = false;
            continue;
        }
        else if (token == "vt")
        {
            beginTex = true;
            hasTexCoordInfo = true;
            beginNor = false;
            continue;
        }
        else if (token == "s")
        {
            line >> token;
            continue;
        }
        else if (token == "o")
        {
            line >> token;
            beginNam = true;
            continue;
        }
        else if (token == "f")
        {
            beginIdx = true;
            beginTex = false;
            continue;
        }

        if (beginPos && !beginNam)
        {
            v.Position[vecPos++] = stof(token);

            if (vecPos > 2)
            {
                positions.push_back(v.Position);
                vecPos = 0;
            }
        }
        else if (beginNor && !beginTex && !beginIdx)
        {
            v.Normal[vecPos++] = stof(token);

            if (vecPos > 2)
            {
                normals.push_back(v.Normal);
                vecPos = 0;
            }
        }
        else if (beginTex && !beginNor && !beginIdx)
        {
            v.TexCoord[vecPos++] = stof(token);

            if (vecPos > 1)
            {
                texcoords.push_back(v.TexCoord);
                vecPos = 0;
            }

        }
        else if (beginIdx)
        {
            glm::vec3 indexData;

            string tmp = "";

            std::replace(token.begin(), token.end(), '/', ' ');

            stringstream val;

            val << token;

            string valToken;

            while (val >> valToken)
            {
                if (hasTexCoordInfo)
                {
                    indexData[vecPos++] = stoi(valToken);

                    if (vecPos > 2)
                    {
                        vecPos = 0;
                    }
                }
                else
                {
                    indexData[0] = stoi(valToken);
                    indexData[1] = 0;

                    val >> valToken;
                    indexData[2] = stoi(valToken);
                }
            }

            Vertex fixedVert{};

            if (hasTexCoordInfo)
            {

                fixedVert.Position = glm::vec3(positions[indexData[0] - 1]);
                fixedVert.TexCoord = glm::vec2(texcoords[indexData[1] - 1]);
                fixedVert.Normal = glm::vec3(normals[indexData[2] - 1]);
            }
            else
            {
                fixedVert.Position = glm::vec3(positions[indexData[0] - 1]);
                fixedVert.TexCoord = glm::vec2(0, 0);
                fixedVert.Normal = glm::vec3(normals[indexData[2] - 1]);

            }

            mesh.push_back(fixedVert);

            indices.push_back(indexData);
        }
    }

    if (mesh.empty()) {
        cerr << "No vertices loaded from file: " << fileName << endl;
        return;
    }

    meshes.push_back(mesh);

    GLuint tempVAO;

    GLuint tempVBO;

    GLuint tempShaderProgram = 0;

    renderModel(tempVAO, tempVBO, tempShaderProgram, mesh);
}

class Scene : public wxFrame {
public:
    Scene();

private:
    Renderer* m_canvas;
};

Scene::Scene()
        : wxFrame(nullptr, wxID_ANY, "CMEngine", wxDefaultPosition, wxSize(800, 600)) {

    SetBackgroundColour(wxColour(45, 45, 48));
    m_canvas = new Renderer(this);

    wxWindow::SetDropTarget(new DropTarget(m_canvas));

#ifdef _WIN32
    // Get the native window handle (HWND) of the wxWidgets frame
    HWND hwnd = static_cast<HWND>(GetHandle());

    // Apply dark mode to the window
    SetDarkModeForWindow(hwnd);
#endif
}

class App : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(App);

bool App::OnInit() {
    Scene* frame = new Scene();
    frame->Show(true);
    return true;
}

#endif // WINDOW_H
