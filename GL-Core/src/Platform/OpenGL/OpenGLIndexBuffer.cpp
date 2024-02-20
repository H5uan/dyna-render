#include "OpenGLIndexBuffer.h"

#include <glad/glad.h>

OpenGLIndexBuffer::OpenGLIndexBuffer(const std::vector<uint32_t>&indices) : mCount(indices.size()) {
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, sizeof(uint32_t) * mCount, indices.data(),
                      GL_STATIC_DRAW);
}

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count): mCount(count) {
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, sizeof(uint32_t) * count, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void OpenGLIndexBuffer::UpdateData(const std::vector<uint32_t>&indices) {
    mCount = indices.size();
    glBufferSubData(mRendererID, 0, sizeof(uint32_t) * mCount, indices.data());
}
