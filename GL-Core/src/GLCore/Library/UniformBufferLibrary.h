#pragma once

#include "GLCore/Rendering/UniformBuffer.h"
#include "GLCore/Library/Library.h"

#include <glm/glm.hpp>

struct CameraData {
    glm::mat4 ViewProjection;
};

template<>
class Library<UniformBuffer> : public LibraryBase<Library, UniformBuffer> {
public:
    Library();

    [[nodiscard]] Ref<UniformBuffer> GetCameraUniformBuffer();
};
