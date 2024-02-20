#pragma once

#include "GLCore/Rendering/VertexBuffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer() = default;

    OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);

    OpenGLVertexBuffer(const std::vector<std::any>&vertices,
                       VertexBufferUsage usage = VertexBufferUsage::Static);

    ~OpenGLVertexBuffer() override;

    void UpdataData(const std::vector<std::any>&vertices) override;

    [[nodiscard]] const BufferLayout& GetLayout() const override { return mLayout; }
    void SetLayout(const BufferLayout&layout) override { mLayout = layout; }

    [[nodiscard]] uint32_t GetID() const override { return mRendererID; }

private:
    uint32_t mRendererID;
    VertexBufferUsage mUsage;
    BufferLayout mLayout;
};
