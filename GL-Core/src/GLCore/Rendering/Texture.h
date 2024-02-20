#pragma once
#include <cstdint>
#include <string>

#include "GLCore/Core/Base.h"

namespace std::filesystem {
    class path;
}

class Texture {
public:
    virtual ~Texture() = default;

    [[nodiscard]] virtual uint32_t GetWidth() const = 0;

    [[nodiscard]] virtual uint32_t GetHeight() const = 0;

    [[nodiscard]] virtual uint32_t GetRendererID() const = 0;

    virtual void SetData(void* data, uint32_t size) {
    };

    virtual void Bind(uint32_t slot = 0) const = 0;

    virtual void UnBind() const = 0;

    [[nodiscard]] virtual bool IsLoaded() const { return false; };

    virtual bool operator==(const Texture&other) const = 0;
};

class Texture2D : public Texture {
public:
    static Ref<Texture2D> Create(uint32_t width, uint32_t height);

    static Ref<Texture2D> Create(const std::filesystem::path&path);

    static Ref<Texture2D> Create(const std::string&path);
};

