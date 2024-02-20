#pragma once

#include <filesystem>

#include "GLCore/Rendering/Texture.h"

#include <glad/glad.h>

class OpenGLTexture2D : public Texture2D {
public:
    OpenGLTexture2D(uint32_t width, uint32_t height);

    OpenGLTexture2D(const std::filesystem::path&path);

    ~OpenGLTexture2D() override;

    uint32_t GetWidth() const override { return mWidth; };
    uint32_t GetHeight() const override { return mHeight; };
    uint32_t GetRendererID() const override { return mRendererID; }

    void SetData(void* data, uint32_t size) override;

    void Bind(uint32_t slot = 0) const override;

    void UnBind() const override;

    bool IsLoaded() const override { return mIsLoaded; }

    bool operator==(const Texture&other) const override {
        return mRendererID == ((OpenGLTexture2D &)other).mRendererID;
    }

private:
    std::filesystem::path mPath;
    bool mIsLoaded = false;
    uint32_t mWidth, mHeight;
    uint32_t mRendererID;
    GLenum mInternalFormat, mDataFormat;
};
