#include "shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    int success = 0;
    char infoLog[512];

    // Compilazione e binding come da codice precedente
    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vShaderCode, NULL);
    glCompileShader(vert);

    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vert, 512, NULL, infoLog);
        cerr << "Compile error in Vertex Shader: " << infoLog;
    }

    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fShaderCode, NULL);
    glCompileShader(frag);

    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag, 512, NULL, infoLog);
        cerr << "Compile error in Fragment Shader: " << infoLog;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vert);
    glAttachShader(ID, frag);
    glLinkProgram(ID);
// print linking errors if any
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

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