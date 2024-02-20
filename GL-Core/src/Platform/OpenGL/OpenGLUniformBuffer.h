#pragma once

#include <any>
#include <vector>

#include "GLCore/Rendering/UniformBuffer.h"

class OpenGLUniformBuffer : public UniformBuffer {
public:
    OpenGLUniformBuffer(uint32_t size, uint32_t binding);

    ~OpenGLUniformBuffer() override;

    void SetData(const std::vector<std::any>&uniform_data, uint32_t offset) override;

private:
    uint32_t mRendererID = 0;
};
