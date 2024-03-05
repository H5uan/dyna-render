#include "Texture.h"

#include <filesystem>

#include "GLCore/Resource/AssetManager.h"
#include "Platform/OpenGL/OpenGLTexture.h"

Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
    return CreateRef<OpenGLTexture2D>(width, height);
}

Ref<Texture2D> Texture2D::Create(const std::filesystem::path&path) {
    return CreateRef<OpenGLTexture2D>(path);
}

Ref<Texture2D> Texture2D::Create(const std::string&path) {
    return Create(AssetManager::GetFullPath(path));
}

Ref<CubeMapTexture> CubeMapTexture::Create(std::vector<std::string>&paths) {
    return CreateRef<OpenGLCubeMapTexture>(paths);
}

Ref<CubeMapTexture> CubeMapTexture::Create() {
    return CreateRef<OpenGLCubeMapTexture>();
}

Ref<CubeMapTexture> CubeMapTexture::Create(uint32_t width, uint32_t height) {
    return CreateRef<OpenGLCubeMapTexture>(width, height);
}
