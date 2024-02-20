#include "NewShader.h"

#include <filesystem>
#include <utility>

#include "Platform/OpenGL/NativeOpenGLShader.h"
#include "Platform/OpenGL/OpenGLShader.h"
/***
Ref<NewShader> NewShader::Create(const std::filesystem::path&filepath) {
    return Create(filepath.string());
}

// TODO: DX12 and spir-v support
Ref<NewShader> NewShader::Create(const std::string&filepath) {
    return CreateRef<OpenGLShader>(filepath);
}

Ref< NewShader> NewShader::CreateNative(const std::filesystem::path&filepath) {
    return CreateNative(filepath.string());
}

Ref< NewShader> NewShader::CreateNative(const std::string&name, const std::string&vertexSrc, const std::string&fragmentSrc) {
    return CreateRef<NativeOpenGLShader>();
}

 NewShaderUniform:: NewShaderUniform(std::string name, NewShaderUniformType type, uint32_t size,
                             uint32_t offset): m_Name(std::move(name)), m_Type(type), m_Size(size), m_Offset(offset) {
}

const std::string& NewShaderUniform::UniformTypeToString( NewShaderUniformType type) {
    if (type == NewShaderUniformType::Bool) {
        return "Boolean";
    }
    else if (type == NewShaderUniformType::Int) {
        return "Int";
    }
    else if (type == NewShaderUniformType::Float) {
        return "Float";
    }

    return "None";
}
***/