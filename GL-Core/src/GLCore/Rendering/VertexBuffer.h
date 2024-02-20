#pragma once
#include <any>
#include <cstdint>
#include <utility>

#include "GLCore.h"
#include "GLCore/Core/Base.h"

enum class ShaderDataType {
    None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};

struct ShaderDataTypeInfo {
    uint32_t Size;
    uint32_t ElementCount;
};

static const std::unordered_map<ShaderDataType, ShaderDataTypeInfo> ShaderDataTypeMap = {
    {ShaderDataType::Float, {4, 1}},
    {ShaderDataType::Float2, {4 * 2, 2}},
    {ShaderDataType::Float3, {4 * 3, 3}},
    {ShaderDataType::Float4, {4 * 4, 4}},
    {ShaderDataType::Mat3, {4 * 3 * 3, 3 * 3}},
    {ShaderDataType::Mat4, {4 * 4 * 4, 4 * 4}},
    {ShaderDataType::Int, {4, 1}},
    {ShaderDataType::Int2, {4 * 2, 2}},
    {ShaderDataType::Int3, {4 * 3, 3}},
    {ShaderDataType::Int4, {4 * 4, 4}},
    {ShaderDataType::Bool, {1, 1}}
};

static uint32_t ShaderDataTypeSize(const ShaderDataType type) {
    return ShaderDataTypeMap.at(type).Size;
}

struct BufferElement {
    std::string Name{};
    ShaderDataType Type{};
    uint32_t Size{};
    uint32_t Offset{}; // distance bettwen different attrib
    bool Normalized{};

    BufferElement() = default;

    BufferElement(const ShaderDataType type, std::string name, const bool normalized = false)
        : Name(std::move(name)), Type(type), Size(ShaderDataTypeMap.at(type).Size), Offset(0), Normalized(normalized) {
    }

    [[nodiscard]] uint32_t GetComponentCount() const {
        return ShaderDataTypeMap.at(Type).ElementCount;
    }
};

class BufferLayout {
public:
    BufferLayout() = default;

    BufferLayout(const std::initializer_list<BufferElement>&elements)
        : mElements(elements) {
        CalculateOffsetsAndStride();
    }

    [[nodiscard]] uint32_t GetStride() const { return mStride; }
    [[nodiscard]] const std::vector<BufferElement>& GetElements() const { return mElements; }

    std::vector<BufferElement>::iterator begin() { return mElements.begin(); }
    std::vector<BufferElement>::iterator end() { return mElements.end(); }
    [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return mElements.begin(); }
    [[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return mElements.end(); }

private:
    void CalculateOffsetsAndStride() {
        uint32_t offset = 0;
        mStride = 0;
        for (auto&element: mElements) {
            element.Offset = offset;
            offset += element.Size;
            mStride += element.Size;
        }
    }

    std::vector<BufferElement> mElements;
    uint32_t mStride = 0; // distance between the same vertices' attribs
};

enum class VertexBufferUsage {
    None = 0, Static = 1, Dynamic = 2
};


class VertexBuffer {
public:
    virtual ~VertexBuffer() = default;

    virtual void UpdataData(const std::vector<std::any>&data) = 0;

    [[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;

    virtual void SetLayout(const BufferLayout&layout) = 0;

    [[nodiscard]] virtual uint32_t GetID() const =0;

    static Ref<VertexBuffer> Create(uint32_t size, VertexBufferUsage usage = VertexBufferUsage::Dynamic);

    static Ref<VertexBuffer> Create(const std::vector<std::any>&vertices,
                                    VertexBufferUsage usage = VertexBufferUsage::Static);
};
