#pragma once

#include "Framebuffer.h"
#include "VertexArray.h"
#include "Texture.h"

enum class PostProcessingType {
    None = 0,
    MSAA = 1,
    GaussianBlur,
    SSAO,
};

class PostProcessing {
public:
    PostProcessing(const PostProcessing&type);

    virtual ~PostProcessing();

public:

};
