#include "VertexBuffer.h"

#include "Platform/OpenGL/OpenGLVertexBuffer.h"


// TODO: DX12 support
Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage) {
    return CreateRef<OpenGLVertexBuffer>();
}

Ref<VertexBuffer> VertexBuffer::Create(const std::vector<std::any>&vertices, VertexBufferUsage usage) {
    return CreateRef<OpenGLVertexBuffer>(vertices, usage);
}
