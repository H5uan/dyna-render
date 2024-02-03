#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <GLCore/Core/Log.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
    unsigned int m_ID = -1;
    std::string m_VertexPath;
    std::string m_FragmentPath;

    Shader(const char* vertexPath, const char* fragmentPath);

    void Reload();


    void Use() const {
        glUseProgram(m_ID);
    }


    void SetBool(const std::string&name, bool value) const {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
    }


    void SetInt(const std::string&name, int value) const {
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void SetFloat(const std::string&name, float value) const {
        glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
    }

    void SetVec2(const std::string&name, const glm::vec2&value) const {
        glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }

    void SetVec2(const std::string&name, float x, float y) const {
        glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
    }

    void SetVec3(const std::string&name, const glm::vec3&value) const {
        glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }

    void SetVec3(const std::string&name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
    }

    void SetVec4(const std::string&name, const glm::vec4&value) const {
        glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
    }

    void SetVec4(const std::string&name, float x, float y, float z, float w) const {
        glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
    }

    void SetMat2(const std::string&name, const glm::mat2&mat) const {
        glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void SetMat3(const std::string&name, const glm::mat3&mat) const {
        glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void SetMat4(const std::string&name, const glm::mat4&mat) const {
        glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

private:
    std::string ReadShaderCode(const char* shaderPath);

    unsigned int CompileShader(GLenum type, const char* code);

    unsigned int LinkProgram(unsigned int vertex, unsigned int fragment);

    void CheckCompileErrors(GLuint shader, std::string type);
};
