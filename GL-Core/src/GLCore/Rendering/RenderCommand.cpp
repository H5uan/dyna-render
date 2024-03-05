#include "RenderingCommand.h"

#include "Platform/OpenGL/OpenGLRenderingAPI.h"

#include <GLCore/Core/Base.h>


Scope<RenderingAPI> RenderCommand::sRendererAPI = CreateScope<OpenGLRenderingAPI>();
