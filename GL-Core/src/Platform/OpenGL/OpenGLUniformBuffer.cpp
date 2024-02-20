#include "OpenGLUniformBuffer.h"

#include <glad/glad.h>

OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, size, nullptr, GL_DYNAMIC_DRAW); // TODO: investigate usage hint
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, mRendererID);
}

OpenGLUniformBuffer::~OpenGLUniformBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void OpenGLUniformBuffer::SetData(const std::vector<std::any>&uniform_data, uint32_t offset) {
    glNamedBufferSubData(mRendererID, offset, sizeof(uniform_data[0]) * uniform_data.size(), uniform_data.data());
}
