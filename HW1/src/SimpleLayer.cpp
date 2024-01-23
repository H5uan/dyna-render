#include "SimpleLayer.h"

#include <glfw/glfw3.h>

#include "GLCore/Core/KeyCodes.h"

using namespace GLCore;
using namespace GLCore::Utils;

SimpleLayer::SimpleLayer() = default;

SimpleLayer::~SimpleLayer() = default;

void SimpleLayer::OnAttach() {
    EnableGLDebugging();
}

void SimpleLayer::OnDetach() {
}

void SimpleLayer::OnEvent(Event&event) {
}

void SimpleLayer::OnUpdate(Timestep ts) {
    const double time_value = glfwGetTime();
    const double green_value = sin(time_value);
    glClearColor(0.2f, static_cast<GLfloat>(green_value), 0.3f, 1.0f); // change the green component according to sin function
    glClear(GL_COLOR_BUFFER_BIT);
}

void SimpleLayer::OnImGuiRender() {
}
