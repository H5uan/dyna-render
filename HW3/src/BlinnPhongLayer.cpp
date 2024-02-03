#include "BlinnPhongLayer.h"

#include <glfw/glfw3.h>

#include <utility>

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include "GLCore/Core/MouseButtonCodes.h"

using namespace GLCore;
using namespace GLCore::Utils;

BlinnPhongLayer::BlinnPhongLayer(std::string obj_file_path): m_ObjFilePath(std::move(obj_file_path)),
                                                             m_OrbitCamera(45.0f, 16.0f / 9.0f, 1.0f, 1000.0f),
                                                             m_OrthographicCamera(16.0f / 9.0f, true, false) {
}

void BlinnPhongLayer::OnAttach() {
    EnableGLDebugging();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    LOG_INFO("OBJ PATH: ", m_ObjFilePath);
    LOG_INFO("  {0}", m_ObjFilePath);


    m_BlinnPhongShader = new Shader(
        "../../../HW3/assets/shaders/Blinn-Phong.vert.glsl",
        "../../../HW3/assets/shaders/Blinn-Phong.frag.glsl"
    );

    m_lightCubeShader = new Shader(
        "../../../HW3/assets/shaders/LightCube.vert.glsl",
        "../../../HW3/assets/shaders/LightCube.frag.glsl"
    );

    m_Model = new Model(m_ObjFilePath);

    glGenVertexArrays(1, &lightCubeVAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(lightCubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &lightCubeEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightCubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);
}

void BlinnPhongLayer::OnDetach() {
    delete m_Model;
    delete m_BlinnPhongShader;
    delete m_lightCubeShader;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void BlinnPhongLayer::OnEvent(Event&event) {
    m_OrbitCamera.OnEvent(event);

    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<KeyPressedEvent>([&](const KeyPressedEvent&e) {
        if (e.GetKeyCode() == static_cast<int>(KeyCode::F6))
            m_BlinnPhongShader->Reload();
        return false;
    });

    dispatcher.Dispatch<KeyPressedEvent>([&](const KeyPressedEvent&e) {
        if (e.GetKeyCode() == static_cast<int>(KeyCode::LeftControl) || e.GetKeyCode() == static_cast<int>(
                KeyCode::RightControl)) {
            ctrlPressed = true;
        }
        return false;
    });
    dispatcher.Dispatch<KeyReleasedEvent>([&](const KeyReleasedEvent&e) {
        if (e.GetKeyCode() == static_cast<int>(KeyCode::LeftControl) || e.GetKeyCode() == static_cast<int>(
                KeyCode::RightControl)) {
            ctrlPressed = false;
        }
        return false;
    });
    dispatcher.Dispatch<MouseButtonPressedEvent>([&](const MouseButtonPressedEvent&e) {
        if (ctrlPressed && e.GetMouseButton() == static_cast<int>(MouseButton::Left)) {
            auto [x, y] = Input::GetMousePosition();
            lastMousePos = glm::vec2(x, y);
        }
        return false;
    });
    dispatcher.Dispatch<MouseMovedEvent>([&](const MouseMovedEvent&e) {
        if (ctrlPressed) {
            glm::vec2 mousePos(e.GetX(), e.GetY());
            glm::vec2 delta = mousePos - lastMousePos;
            lastMousePos = mousePos;

            azimuth += delta.x * m_Sensitivity; // 允许方位角自由旋转
            elevation -= delta.y * m_Sensitivity; // 控制仰角在一定范围内

            // 限制仰角以防翻转，但不限制方位角
            elevation = glm::clamp(elevation, -89.0f, 89.0f);
        }
        return false;
    });
}

void BlinnPhongLayer::OnUpdate(const Timestep ts) {
    m_OrbitCamera.OnUpdate(ts);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_BlinnPhongShader->Use();
    glm::vec3 center = m_Model->meshes[0].m_Center;
    const glm::mat4 model = glm::translate(glm::mat4(1.0f), -center);
    m_Light.m_Position.x = radius * cos(glm::radians(elevation)) * cos(glm::radians(azimuth));
    m_Light.m_Position.y = radius * sin(glm::radians(elevation));
    m_Light.m_Position.z = radius * cos(glm::radians(elevation)) * sin(glm::radians(azimuth));
    m_BlinnPhongShader->SetMat4("u_Model", model);
    m_BlinnPhongShader->SetMat4("u_ViewProjection",
                                m_OrbitCamera.GetViewProjection());
    m_BlinnPhongShader->SetVec3("u_Color", m_Material.m_Color);
    m_BlinnPhongShader->SetVec3("lightPos", m_Light.m_Position);
    m_BlinnPhongShader->SetVec3("viewPos", m_OrbitCamera.GetPosition());
    m_BlinnPhongShader->SetBool("blinn", true);
    m_Model->Draw(*m_BlinnPhongShader);

    glm::mat4 new_model = glm::translate(glm::mat4(1.0f), m_Light.m_Position);
    new_model = glm::scale(new_model, glm::vec3(1.0f));
    m_lightCubeShader->Use();
    m_lightCubeShader->SetMat4("u_Model", new_model);
    m_lightCubeShader->SetMat4("u_ViewProjection", m_OrbitCamera.GetViewProjection());
    glBindVertexArray(lightCubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void BlinnPhongLayer::OnImGuiRender() {
    ImGui::Begin("Properties");

    if (ImGui::CollapsingHeader("Light Properties")) {
        ImGui::DragFloat("X Position", &m_Light.m_Position.x, 0.1f, -100.0f, 100.0f, "X: %.1f");
        ImGui::DragFloat("Y Position", &m_Light.m_Position.y, 0.1f, -100.0f, 100.0f, "Y: %.1f");
        ImGui::DragFloat("Z Position", &m_Light.m_Position.z, 0.1f, -100.0f, 100.0f, "Z: %.1f");
        ImGui::SliderFloat("Radius", &radius, 0.0f, 100.0f, "Radius = %.2f");
    }

    if (ImGui::CollapsingHeader("Material Properties")) {
        ImGui::ColorEdit3("Material Color", glm::value_ptr(m_Material.m_Color)); // 假设 m_Material 是您的材质实例
    }

    if (ImGui::CollapsingHeader("Control Settings")) {
        ImGui::SliderFloat("Sensitivity", &m_Sensitivity, 0.01f, 1.0f, "Sensitivity = %.3f");
    }

    ImGui::End();
}
