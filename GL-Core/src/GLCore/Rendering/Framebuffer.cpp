#include "Framebuffer.h"
#include <GLCore/Core/Base.h>

#include "../../Platform/OpenGL/OpenGLFramebuffer.h"

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification&spec) {
    // Currently only support opengl
    // TODO: DirectX12 support
    return CreateRef<OpenGLFramebuffer>(spec);
}
