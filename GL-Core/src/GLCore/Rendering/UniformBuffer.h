#pragma once
#include <any>
#include <cstdint>
#include <vector>

#include "GLCore/Core/Base.h"

class UniformBuffer {
public:
    virtual ~UniformBuffer() = default;

    virtual void SetData(const std::vector<std::any>&data, uint32_t offset = 0) = 0;

    static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding);
};
