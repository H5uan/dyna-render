#pragma once
#include <cstdint>
#include <vector>

#include "GLCore/Core/Base.h"

class IndexBuffer {
public:
    virtual ~IndexBuffer() = default;

    virtual void UpdateData(const std::vector<uint32_t>&indices) = 0;

    [[nodiscard]] virtual uint32_t GetCount() const = 0;

    [[nodiscard]] virtual uint32_t GetID() const = 0;

    static Ref<IndexBuffer> Create(uint32_t count);

    static Ref<IndexBuffer> Create(const std::vector<uint32_t>&indices, uint32_t count);
};
