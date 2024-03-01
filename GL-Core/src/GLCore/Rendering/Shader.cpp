#include "Shader.h"
#include "Renderer.h"
#include "Platform/OpenGL/NativeOpenGLShader.h"


/***
Ref<Shader> Shader::Create(const std::filesystem::path& filepath)
{

}

Ref<Shader> Shader::Create(const std::string& filepath)
{

}

Ref<Shader> Shader::Create(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
{

}
***/
Ref<Shader> Shader::CreateNative(const std::filesystem::path&filepath) {
    return CreateNative(filepath.string());
}

Ref<Shader> Shader::CreateNative(const std::string&filepath) {
    return std::make_shared<NativeOpenGLShader>(filepath);
}

Ref<Shader> Shader::CreateNative(const std::string&name, const std::string&vertexSrc,
                                 const std::string&fragmentSrc) {
    return std::make_shared<NativeOpenGLShader>(
        name, vertexSrc, fragmentSrc);
}

ShaderUniform::ShaderUniform(const std::string&name, ShaderUniformType type, uint32_t size, uint32_t offset)
    : mName(name), mType(type), mSize(size), mOffset(offset) {
}

const std::string& ShaderUniform::UniformTypeToString(ShaderUniformType type) {
    if (type == ShaderUniformType::Bool) {
        return "Boolean";
    }
    else if (type == ShaderUniformType::Int) {
        return "Int";
    }
    else if (type == ShaderUniformType::Float) {
        return "Float";
    }

    return "None";
}
