#include "Framebuffer.h"
#include <GLCore/Core/Base.h>

#include "../../Platform/OpenGL/OpenGLFramebuffer.h"

Ref<Framebuffer> Framebuffer::Create(const FramebufferSpecification&spec) {
    return CreateRef<OpenGLFramebuffer>(spec);
}
