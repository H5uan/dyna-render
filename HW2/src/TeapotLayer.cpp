#include "TeapotLayer.h"

#include <glfw/glfw3.h>

#include <utility>

#include "GLCore/Core/KeyCodes.h"

using namespace GLCore;
using namespace GLCore::Utils;

TeapotLayer::TeapotLayer(std::string obj_file_path): m_ObjFilePath(std::move(obj_file_path)),
                                                     m_OrbitCamera(45.0f, 16.0f / 9.0f, 1.0f, 1000.0f),
                                                     m_OrthographicCamera(16.0f / 9.0f, true, false) {
}

void TeapotLayer::OnAttach() {
    EnableGLDebugging();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    LOG_INFO("OBJ PATH: ", m_ObjFilePath);
    LOG_INFO("  {0}", m_ObjFilePath);


    m_Shader = new Shader(
        "../../HW2/assets/shaders/teapot.vert.glsl",
        "../../HW2/assets/shaders/teapot.frag.glsl"
    );

    m_Model = new Model(m_ObjFilePath);
}

void TeapotLayer::OnDetach() {
}

void TeapotLayer::OnEvent(Event&event) {
    m_OrbitCamera.OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent&e) {
        if (e.GetKeyCode() == static_cast<int>(KeyCode::F6))
            m_Shader->Reload();
        return false;
    });
}

void TeapotLayer::OnUpdate(const Timestep ts) {
    m_OrbitCamera.OnUpdate(ts);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_Shader->Use();
    m_Shader->SetMat4("u_ViewProjection",
                      m_OrbitCamera.GetViewProjection());
    glm::vec3 center = m_Model->meshes[0].m_Center;
    //LOG_INFO("CENTER {0} {1} {2}", center.x, center.y, center.z);

    const glm::mat4 model = glm::translate(glm::mat4(1.0f), -center);

    m_Shader->SetMat4("u_Model", model);

    m_Model->Draw(*m_Shader, true);
}

void TeapotLayer::OnImGuiRender() {
}
