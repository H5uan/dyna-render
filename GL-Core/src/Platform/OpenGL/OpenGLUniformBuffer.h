#pragma once

#include <any>
#include <vector>

#include "GLCore/Rendering/UniformBuffer.h"

class OpenGLUniformBuffer : public UniformBuffer {
public:
    OpenGLUniformBuffer(uint32_t size, uint32_t binding);

    virtual ~OpenGLUniformBuffer();

    void SetData(const void* data, uint32_t size, uint32_t offset = 0) override;

private:
    uint32_t m_RendererID = 0;
};
