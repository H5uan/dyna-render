#pragma once

#include "GLCore/Rendering/NewShader.h"

#include <glm/glm.hpp>
#include <unordered_map>
/***
class NativeOpenGLShader : public Shader {
public:
    NativeOpenGLShader(const std::string&filepath);

    NativeOpenGLShader(std::string name, const std::string&vertexSrc, const std::string&fragmentSrc);

    ~NativeOpenGLShader() override;

    void Bind() const override;

    void Unbind() const override;

    void SetBool(const std::string&name, bool value) override;

    void SetInt(const std::string&name, int value) override;

    void SetIntArray(const std::string&name, int* values, uint32_t count) override;

    void SetFloat(const std::string&name, float value) override;

    void SetFloat3(const std::string&name, const glm::vec3&value) override;

    void SetFloat4(const std::string&name, const glm::vec4&value) override;

    void SetMat4(const std::string&name, const glm::mat4&value) override;

    const std::string& GetName() const override { return mName; };

    void UploadUniformInt(const std::string&name, int value);

    void UploadUniformIntArray(const std::string&name, int* values, uint32_t count);

    void UploadUniformFloat(const std::string&name, float value);

    void UploadUniformFloat2(const std::string&name, const glm::vec2&value);

    void UploadUniformFloat3(const std::string&name, const glm::vec3&value);

    void UploadUniformFloat4(const std::string&name, const glm::vec4&value);

    void UploadUniformMat3(const std::string&name, const glm::mat3&matrix);

    void UploadUniformMat4(const std::string&name, const glm::mat4&matrix);

private:
    std::string ReadFile(const std::string&filepath);

    std::unordered_map<GLenum, std::string> PreProcess(const std::string&source);

    void Compile(const std::unordered_map<GLenum, std::string>&shaderSources);

private:
    uint32_t mRendererID;
    std::string mName;
};
***/