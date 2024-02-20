#include "IndexBuffer.h"

#include "GLCore/Core/Base.h"
#include "Platform/OpenGL/OpenGLIndexBuffer.h"

Ref<IndexBuffer> IndexBuffer::Create(uint32_t count) {
    return CreateRef<OpenGLIndexBuffer>(count);
}

Ref<IndexBuffer> IndexBuffer::Create(const std::vector<uint32_t>&indices, uint32_t count) {
    return CreateRef<OpenGLIndexBuffer>(indices);
}
