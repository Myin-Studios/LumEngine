#include "Shaders.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath) : vertPath(vertexPath.c_str()), fragPath(fragmentPath.c_str()), geomPath(geometryPath.c_str())
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    if (!std::filesystem::exists(std::filesystem::absolute(std::filesystem::path(vertPath)))) {
        std::cerr << "Vertex shader file does not exist: " << vertPath << std::endl;
        return;
    }
    if (!std::filesystem::exists(std::filesystem::absolute(std::filesystem::path(fragPath)))) {
		std::cerr << "Fragment shader file does not exist: " << fragPath << std::endl;
        return;
	}
    if (!geometryPath.empty() && !std::filesystem::exists(std::filesystem::absolute(std::filesystem::path(geomPath)))) {
        std::cerr << "Geometry shader file does not exist: " << geomPath << std::endl;
        return;
    }

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(std::filesystem::absolute(std::filesystem::path(vertPath)).string());
		fShaderFile.open(std::filesystem::absolute(std::filesystem::path(fragPath)).string());

		std::stringstream vs;
		vs << vShaderFile.rdbuf();

		std::stringstream fs;
		fs << fShaderFile.rdbuf();

		vertexCode = vs.str();
		fragmentCode = fs.str();

		// close file handlers
		vShaderFile.close();
		fShaderFile.close();

		if (!geometryPath.empty()) {
			gShaderFile.open(std::filesystem::absolute(std::filesystem::path(geomPath)).string());
			std::stringstream gs;
			gs << gShaderFile.rdbuf();
			geometryCode = gs.str();
			gShaderFile.close();
		}
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

    // Compile Geometry Shader
    GLuint geom;
    if (!geometryPath.empty()) {
		geom = glCreateShader(GL_GEOMETRY_SHADER);
		const char* gShaderCode = geometryCode.c_str();
		glShaderSource(geom, 1, &gShaderCode, nullptr);
		glCompileShader(geom);

		glGetShaderiv(geom, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(geom, 512, NULL, infoLog);
			std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}

    // Link Shader Program
    ID = glCreateProgram();
    glAttachShader(ID, vert);
    glAttachShader(ID, frag);

    if (!geometryPath.empty()) {
        glAttachShader(ID, geom);

        glDeleteShader(geom);

        glProgramParameteri(ID, GL_GEOMETRY_INPUT_TYPE, GL_TRIANGLES);
        glProgramParameteri(ID, GL_GEOMETRY_OUTPUT_TYPE, GL_TRIANGLE_STRIP);
        glProgramParameteri(ID, GL_GEOMETRY_VERTICES_OUT, 6);
    }

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

Shader::Shader(const Shader& s) : vertPath(s.vertPath), fragPath(s.fragPath), ID(s.ID)
{
    
}

Shader::Shader(Shader&& s) noexcept
    : vertPath(std::move(s.vertPath)), fragPath(std::move(s.fragPath)), ID(std::move(s.ID))
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    if (!std::filesystem::exists(std::filesystem::absolute(std::filesystem::path(vertPath)))) {
        std::cerr << "Vertex shader file does not exist: " << vertPath << std::endl;
    }
    if (!std::filesystem::exists(std::filesystem::absolute(std::filesystem::path(fragPath)))) {
        std::cerr << "Fragment shader file does not exist: " << fragPath << std::endl;
    }

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(std::filesystem::absolute(std::filesystem::path(vertPath)).string());
        fShaderFile.open(std::filesystem::absolute(std::filesystem::path(fragPath)).string());

        std::stringstream vs;
        vs << vShaderFile.rdbuf();

        std::stringstream fs;
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

void Shader::setBool(const std::string& name, bool value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1i(loc, (int)value);

    if (loc < 0)
        std::cerr << "Uniform '" << name.c_str() << "' --- Value: " << loc << std::endl;
}

void Shader::setInt(const std::string& name, int value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1i(loc, value);

    if (loc < 0)
        std::cerr << "Uniform '" << name.c_str() << "' --- Value: " << loc << std::endl;
}

void Shader::setFloat(const std::string& name, float value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1f(loc, value);

    if (loc < 0)
        std::cerr << "Uniform '" << name.c_str() << "' --- Value: " << loc << std::endl;
}

void Shader::setFloatArray(const std::string& name, GLsizei count, const float* value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform1fv(loc, count, value);

    if (loc < 0)
        std::cerr << "Uniform '" << name.c_str() << "' --- Value: " << loc << std::endl;
}

void Shader::setVec3(const std::string& name, glm::vec3 v3) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform3f(loc, v3.x, v3.y, v3.z);

    if (loc < 0)
        std::cerr << "Uniform '" << name.c_str() << "' --- Value: " << loc << std::endl;
}

void Shader::setMat4x4(const std::string& name, const float* value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, value);

    if (loc < 0)
        std::cerr << "Uniform '" << name.c_str() << "' --- Value: " << loc << std::endl;
}

void Shader::setVec3Array(const std::string& name, GLsizei count, const float* value) const
{
    GLint loc = glGetUniformLocation(ID, name.c_str());
    glUniform3fv(loc, count, value);

    if (loc < 0)
        std::cerr << "Uniform '" << name.c_str() << "' --- Value: " << loc << std::endl;
}

void Shader::setVec4(const std::string& name, glm::vec4 v4) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform4f(loc, v4.x, v4.y, v4.z, v4.w);

	if (loc < 0)
		std::cerr << "Uniform '" << name.c_str() << "' --- Value: " << loc << std::endl;
}

void Shader::setVec4Array(const std::string& name, GLsizei count, const float* value) const
{
	GLint loc = glGetUniformLocation(ID, name.c_str());
	glUniform4fv(loc, count, value);

	if (loc < 0)
		std::cerr << "Uniform '" << name.c_str() << "' --- Value: " << loc << std::endl;
}

Shader& Shader::operator=(const Shader& s) {
    // Verifica l'auto-assegnazione
    if (this != &s) {
        // Copia dei dati membri
        vertPath = s.vertPath;
        fragPath = s.fragPath;
        ID = s.ID;

        // Se ci sono risorse dinamiche o gestione delle risorse da copiare, aggiungi qui
        // Ad esempio, se Shader gestisce risorse come i programmi OpenGL, dovresti copiare anche quelle.
    }
    return *this; // Restituisce *this per permettere l'assegnazione a catena
}

Shader& Shader::operator=(Shader&& s) noexcept {
    if (this != &s) {
        // Sposta i membri
        vertPath = std::move(s.vertPath);
        fragPath = std::move(s.fragPath);
        ID = std::move(s.ID);

        // Se ci sono risorse dinamiche, spostale anche qui
    }
    return *this;
}