#include "Texture.h"

#include <filesystem>

#include "Platform/OpenGL/OpenGLTexture.h"

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
    return CreateRef<OpenGLTexture2D>(width, height);
}

Ref<Texture2D> Texture2D::Create(const std::filesystem::path&path) {
    return CreateRef<OpenGLTexture2D>(path);
}


