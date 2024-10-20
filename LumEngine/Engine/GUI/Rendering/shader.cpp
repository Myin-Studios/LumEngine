#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    if (!std::filesystem::exists(vertexPath)) {
        std::cerr << "Vertex shader file does not exist: " << vertexPath << std::endl;
    }
    if (!std::filesystem::exists(fragmentPath)) {
        std::cerr << "Fragment shader file does not exist: " << fragmentPath << std::endl;
    }

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        stringstream vs;
        vs << vShaderFile.rdbuf();

        stringstream fs;
        fs << fShaderFile.rdbuf();

        vertexCode = vs.str();
        fragmentCode = fs.str();

        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        return; // Esci dalla funzione se c'è un errore
    }

    // Compile Vertex Shader
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    const char* vShaderCode = vertexCode.c_str();
    glShaderSource(vert, 1, &vShaderCode, nullptr);
    glCompileShader(vert);

    int success;
    char infoLog[512];
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile Fragment Shader
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fShaderCode = fragmentCode.c_str();
    glShaderSource(frag, 1, &fShaderCode, nullptr);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vert);
    glAttachShader(ID, frag);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Clean up shaders
    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::use() const
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1i(loc, (int)value);

    if (loc < 0)
        cerr << "Uniform '" << name.c_str() << "' --- Value: " <<  loc << endl;
}

void Shader::setInt(const std::string &name, int value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1i(loc, value);

    if (loc < 0)
        cerr << "Uniform '" << name.c_str() << "' --- Value: " <<  loc << endl;
}

void Shader::setFloat(const std::string &name, float value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1f(loc, value);

    if (loc < 0)
        cerr << "Uniform '" << name.c_str() << "' --- Value: " <<  loc << endl;
}

void Shader::setMat4x4(const std::string &name, const float* value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);

    if (loc < 0)
        cerr << "Uniform '" << name.c_str() << "' --- Value: " <<  loc << endl;
}