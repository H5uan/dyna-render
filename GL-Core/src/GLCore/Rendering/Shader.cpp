#include "Shader.h"

#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) : m_VertexPath(vertexPath),
                                                                   m_FragmentPath(fragmentPath) {
    Reload();
}


void Shader::Reload() {
    std::string vertexCode = ReadShaderCode(m_VertexPath.c_str());
    std::string fragmentCode = ReadShaderCode(m_FragmentPath.c_str());
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vShaderCode);
    unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fShaderCode);

    unsigned int new_ID = LinkProgram(vertex, fragment);
    if (new_ID) {
        if (m_ID != -1)
            glDeleteProgram(m_ID);
        m_ID = new_ID;
    }
}

std::string Shader::ReadShaderCode(const char* shaderPath) {
    std::ifstream shaderFile(shaderPath);
    if (!shaderFile) {
        LOG_ERROR("Shader file could not be opened: {0}", shaderPath);
    }
    std::stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
    return shaderStream.str();
}

unsigned int Shader::CompileShader(GLenum type, const char* code) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    CheckCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");
    return shader;
}

unsigned int Shader::LinkProgram(unsigned int vertex, unsigned int fragment) {
    const unsigned int program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    CheckCompileErrors(program, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    return program;
}

void Shader::CheckCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            LOG_ERROR("Shdaer program compiling error of type:{0}", infoLog);
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            LOG_ERROR("Shdaer program linking erre of type:{0}", infoLog);
        }
    }
}