#pragma once

#include "GLCore/Rendering/IndexBuffer.h"

class OpenGLIndexBuffer : public IndexBuffer {
public:
    explicit OpenGLIndexBuffer(const std::vector<uint32_t>&indices);

    explicit OpenGLIndexBuffer(uint32_t count);

    ~OpenGLIndexBuffer() override;

    // update data in index buffer
    void UpdateData(const std::vector<uint32_t>&indices) override;

    [[nodiscard]] uint32_t GetCount() const override { return mCount; }

    [[nodiscard]] uint32_t GetID() const override { return mRendererID; }

private:
    uint32_t mRendererID;
    uint32_t mCount;
};
