#pragma once
#include "GLCore/Rendering/GraphicsContext.h"
#include "Platform/Windows/WindowsWindow.h"

class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow* windowHandle);

    void Init() override;
    void SwapBuffers() override;
private:
    GLFWwindow* m_WindowHandle;
};
