#include "OpenGLVertexBuffer.h"

static GLenum OpenGLUsage(VertexBufferUsage usage) {
    switch (usage) {
        case VertexBufferUsage::Static: return GL_STATIC_DRAW;
        case VertexBufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
        default:
            GLCORE_ASSERT(false, "Unknown vertex buffer usage");
    }
    return 0;
}

OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage)
    : mUsage(usage) {
    glCreateBuffers(1, &mRendererID);
    glNamedBufferData(mRendererID, size, nullptr, OpenGLUsage(mUsage)); // set buffer data directly
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const std::vector<std::any>&vertices, VertexBufferUsage usage)
    : mUsage(usage) {
    glCreateBuffers(1, &mRendererID);
    glNamedBufferStorage(mRendererID, vertices.size() * sizeof(vertices[0]), vertices.data(), OpenGLUsage(mUsage));
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &mRendererID);
}

void OpenGLVertexBuffer::UpdataData(const std::vector<std::any>&vertices) {
    glBufferSubData(mRendererID, 0, sizeof(vertices[0]) * vertices.size(), vertices.data());
}
