#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "GLCore/Core/Core.h"

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height): mWidth(width), mHeight(height) {
    mInternalFormat = GL_RGBA8;
    mDataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    glTextureStorage2D(mRendererID, 1, mInternalFormat, mWidth, mHeight);

    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateTextureMipmap(mRendererID);

    GLfloat maxAniso = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
    glTextureParameterf(mRendererID, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path&path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    if (data) {
        mIsLoaded = true;
        mWidth = width;
        mHeight = height;

        // Determine the data format based on channels
        if (channels == 4) {
            mInternalFormat = GL_RGBA8;
            mDataFormat = GL_RGBA;
        }
        else if (channels == 3) {
            mInternalFormat = GL_RGB8;
            mDataFormat = GL_RGB;
        }
        else {
            // Handle other formats or set an error
            GLCORE_ASSERT("Unsupported image format!");
        }

        // Create and set up the texture
        glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
        glTextureStorage2D(mRendererID, 1, mInternalFormat, mWidth, mHeight);
        glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);

        // Set magnification filter to bilinear
        glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set minification filter to use mipmaps with linear filtering
        glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Generate mipmaps
        glGenerateTextureMipmap(mRendererID);

        // Setup anisotropic filtering, if supported
        GLfloat maxAniso = 0.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAniso);
        glTextureParameterf(mRendererID, GL_TEXTURE_MAX_ANISOTROPY, maxAniso);

        stbi_image_free(data);
    }
    else {
        GLCORE_ASSERT("Load Texture Failed!");
    }
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &mRendererID);
}

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
    uint32_t bpp = mDataFormat == GL_RGBA ? 4 : 3;
    glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mDataFormat,GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, mRendererID);
    //glBindTextureUnit(slot, mRendererID);
}

void OpenGLTexture2D::UnBind() const {
    glBindTexture(GL_TEXTURE, 0);
}
