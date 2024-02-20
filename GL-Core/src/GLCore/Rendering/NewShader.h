#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <GLCore/Core/Log.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "GLCore/Core/Base.h"

/***
enum class ShaderUniformType {
    None = 0, Bool, Int, UInt, Float, Vec2, Vec3, Vec4, Mat3, Mat4
};

class ShaderUniform {
public:
    ShaderUniform() = default;

    ShaderUniform(std::string name, ShaderUniformType type, uint32_t size, uint32_t offset);

    [[nodiscard]] const std::string& GetName() const { return m_Name; }
    [[nodiscard]] ShaderUniformType GetType() const { return m_Type; }
    [[nodiscard]] uint32_t GetSize() const { return m_Size; }
    [[nodiscard]] uint32_t GetOffset() const { return m_Offset; }

    static const std::string& UniformTypeToString(ShaderUniformType type);

private:
    std::string m_Name;
    ShaderUniformType m_Type = ShaderUniformType::None;
    uint32_t m_Size = 0;
    uint32_t m_Offset = 0;
};

struct ShaderUniformBuffer {
    std::string Name;
    uint32_t Index;
    uint32_t BindingPoint;
    uint32_t Size;
    uint32_t RendererID;
    std::vector<ShaderUniform> Uniforms;
};

struct ShaderBuffer {
    std::string Name;
    uint32_t Size = 0;
    std::unordered_map<std::string, ShaderUniform> Uniforms;
};

class NewShader {
public:
    virtual ~NewShader() = default;

    virtual void Bind() const = 0;

    virtual void Unbind() const = 0;


    virtual void SetBool(const std::string&name, bool value) = 0;

    virtual void SetInt(const std::string&name, int value) = 0;

    virtual void SetIntArray(const std::string&name, int* values, uint32_t count) = 0;

    virtual void SetFloat(const std::string&name, float value) = 0;

    virtual void SetFloat3(const std::string&name, const glm::vec3&value) = 0;

    virtual void SetFloat4(const std::string&name, const glm::vec4&value) = 0;

    virtual void SetMat4(const std::string&name, const glm::mat4&value) = 0;

    virtual void SetUniform(const std::string&fullname, const glm::mat4&value) {
    };

    [[nodiscard]] virtual const std::string& GetName() const = 0;

    static Ref<Shader> Create(const std::filesystem::path&filepath);

    static Ref<Shader> Create(const std::string&filepath);

    static Ref<Shader> Create(const std::string&name, const std::string&vertexSrc, const std::string&fragmentSrc);

    // Use native shader language instead of spir-v
    static Ref<Shader> CreateNative(const std::filesystem::path&filepath);

    static Ref<Shader> CreateNative(const std::string&filepath);

    static Ref<Shader> CreateNative(const std::string&name, const std::string&vertexSrc, const std::string&fragmentSrc);
};

/***
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
        glUniform1i(glGetUniformLocation(m_ID, name.c_str()), static_cast<int>(value));
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
***/
