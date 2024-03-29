#include "OpenGLTexture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "GLCore/Core/Core.h"
#include "GLCore/Core/Log.h"
#include "GLCore/Resource/AssetManager.h"

OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : mWidth(width), mHeight(height) {
    mInternalFormat = GL_RGBA8;
    mDataFormat = GL_RGBA;

    glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
    glTextureStorage2D(mRendererID, 1, mInternalFormat, mWidth, mHeight);

    glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path&path)
    : mPath(path) {
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

    if (data) {
        mIsLoaded = true;

        mWidth = width;
        mHeight = height;

        GLenum internalFormat = 0, dataFormat = 0;
        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        }
        else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        }
        else if (channels == 1) {
            internalFormat = GL_RGB;
            dataFormat = GL_UNSIGNED_BYTE;
        }

        mInternalFormat = internalFormat;
        mDataFormat = dataFormat;

        GLCORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

        glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
        glTextureStorage2D(mRendererID, 1, internalFormat, mWidth, mHeight);

        glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        GLenum type = internalFormat == GL_RGBA16F ? GL_FLOAT : GL_UNSIGNED_BYTE;
        glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, dataFormat, type, data);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

        stbi_image_free(data);
    }
    else {
        throw "Load Texture Failed!";
    }
}

OpenGLTexture2D::~OpenGLTexture2D() {
    glDeleteTextures(1, &mRendererID);
}

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
    uint32_t bpp = mDataFormat == GL_RGBA ? 4 : 3; // bytes per pixel
    GLCORE_ASSERT(size == mWidth * mHeight * bpp, "Data must be entire texture!");
    glTextureSubImage2D(mRendererID, 0, 0, 0, mWidth, mHeight, mDataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, mRendererID);
    //glBindTextureUnit(slot, mRendererID);
}

void OpenGLTexture2D::UnBind() const {
    glBindTexture(GL_TEXTURE, 0);
}

OpenGLCubeMapTexture::OpenGLCubeMapTexture() {
    glGenTextures(1, &mRendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererID);

    mWidth = 2048;
    mHeight = 2048;

    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, 2048, 2048, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // enable pre-filter mipmap sampling (combatting visible dots artifact)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

OpenGLCubeMapTexture::OpenGLCubeMapTexture(uint32_t width, uint32_t height)
    : mWidth(width), mHeight(height) {
    glGenTextures(1, &mRendererID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererID);

    for (unsigned int i = 0; i < 6; ++i) {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // enable pre-filter mipmap sampling (combatting visible dots artifact)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

OpenGLCubeMapTexture::OpenGLCubeMapTexture(std::vector<std::string>&paths)
    : mPaths(paths) {
    glGenTextures(1, &mRendererID);
    OpenGLCubeMapTexture::Generate();
}

OpenGLCubeMapTexture::~OpenGLCubeMapTexture() {
    glDeleteTextures(1, &mRendererID);
}

void OpenGLCubeMapTexture::SetFace(FaceTarget faceIndex, const std::string&path) {
    mPaths[static_cast<uint32_t>(faceIndex)] = path;
}

void OpenGLCubeMapTexture::GenerateMipmap() {
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void OpenGLCubeMapTexture::Bind(uint32_t slot) const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererID);
}

void OpenGLCubeMapTexture::UnBind() const {
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void OpenGLCubeMapTexture::Generate() {
    glBindTexture(GL_TEXTURE_CUBE_MAP, mRendererID);

    int width = 0;
    int height = 0;
    int nrChannels = 0;
    stbi_set_flip_vertically_on_load(false);
    for (unsigned int i = 0; i < mPaths.size(); i++) {
        unsigned char* data = stbi_load(AssetManager::GetFullPath(mPaths[i]).string().c_str(), &width, &height,
                                        &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else {
            LOG_ERROR("Cubemap doesn't loaded correctly!");
            stbi_image_free(data);
        }
    }

    mWidth = width;
    mHeight = height;

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
