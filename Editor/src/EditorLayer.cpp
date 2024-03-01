#include "EditorLayer.h"

#include <stb_image.h>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm/gtx/quaternion.hpp>


#include <utility>

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include "GLCore/Core/MouseButtonCodes.h"
#include "GLCore/Library/Library.h"
#include "GLCore/Mesh/Mesh.h"
#include "GLCore/Rendering/Framebuffer.h"
#include "GLCore/Rendering/RenderingCommand.h"
#include "GLCore/Rendering/VertexArray.h"

using namespace GLCore;
using namespace GLCore::Utils;

EditorLayer::EditorLayer(std::string obj_file_path): m_ObjFilePath(std::move(obj_file_path)),
                                                     m_EditorCamera(45.0f, 16.0f / 9.0f, 1.0f, 1000.0f) {
}

void EditorLayer::OnAttach() {
    FramebufferSpecification fbSpec;
    fbSpec.Attachments = {
        FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth
    };
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    fbSpec.Samples = false;
    m_Framebuffer = Framebuffer::Create(fbSpec);


    std::vector<std::string> paths{
        "../../../Assets/Textures/Skybox/cubemap_posx.png",
        "../../../Assets/Textures/Skybox/cubemap_negx.png",
        "../../../Assets/Textures/Skybox/cubemap_posy.png",
        "../../../Assets/Textures/Skybox/cubemap_negy.png",
        "../../../Assets/Textures/Skybox/cubemap_posz.png",
        "../../../Assets/Textures/Skybox/cubemap_negz.png",
    };

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_CubeMap);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(paths[0].c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        LOG_ERROR("Cubemap texture failed to load at path: ", paths[0]);
    }
    else {
        glTextureStorage2D(m_CubeMap, 1, GL_RGBA8, width, height);

        for (GLuint i = 0; i < 6; i++) {
            if (i > 0) {
                data = stbi_load(paths[i].c_str(), &width, &height, &nrChannels, 0);
                if (!data) {
                    LOG_ERROR("Cubemap texture failed to load at path: ", paths[i]);
                }
            }

            glTextureSubImage3D(m_CubeMap, 0, 0, 0, i, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
    }

    glTextureParameteri(m_CubeMap, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_CubeMap, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_CubeMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_CubeMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_CubeMap, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    m_Teapot = new Model(m_ObjFilePath);
    m_Plane = new Model("../../../Assets/Models/plane.obj");
    m_Cube = new Model("../../../Assets/Models/cube.obj");
    m_Sphere = new Model("../../../Assets/Models/sphere.obj");
}

void EditorLayer::OnDetach() {
}

void EditorLayer::OnEvent(Event&event) {
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowResizeEvent>([&](const WindowResizeEvent&e) {
            m_ViewportSize = {e.GetWidth(), e.GetHeight()};
            RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
            return false;
        }
    );

    dispatcher.Dispatch<KeyPressedEvent>([&](const KeyPressedEvent&e) {
        switch (e.GetKeyCode()) {
            case static_cast<int>(KeyCode::LeftAlt):
            case static_cast<int>(KeyCode::RightAlt):
                altPressed = true;
                break;
            case static_cast<int>(KeyCode::F6):
                break;
            case static_cast<int>(KeyCode::LeftControl):
            case static_cast<int>(KeyCode::RightControl):
                ctrlPressed = true;
                break;
        }
        return false;
    });

    dispatcher.Dispatch<KeyReleasedEvent>([&](const KeyReleasedEvent&e) {
        switch (e.GetKeyCode()) {
            case static_cast<int>(KeyCode::LeftAlt):
            case static_cast<int>(KeyCode::RightAlt):
                altPressed = false;
                break;
            case static_cast<int>(KeyCode::LeftControl):
            case static_cast<int>(KeyCode::RightControl):
                ctrlPressed = false;
                break;
        }
        return false;
    });

    if (!altPressed) {
        m_EditorCamera.OnEvent(event);
    }


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

            azimuth += delta.x * m_Sensitivity;
            elevation -= delta.y * m_Sensitivity;
            elevation = glm::clamp(elevation, -89.0f, 89.0f);
        }
        return false;
    });
}

void EditorLayer::OnUpdate(const Timestep ts) {
    m_EditorCamera.OnUpdate(ts);


    if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
        m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
        (spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)) {
        m_Framebuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
        m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    }
    RenderCommand::DepthTest(true);


    m_Framebuffer->Bind();
    RenderCommand::SetClearColor({0.4f, 0.4f, 0.4f, 1});
    RenderCommand::Clear();

    m_Light.m_Position.x = radius * cos(glm::radians(elevation)) * cos(glm::radians(azimuth));
    m_Light.m_Position.y = radius * sin(glm::radians(elevation));
    m_Light.m_Position.z = radius * cos(glm::radians(elevation)) * sin(glm::radians(azimuth));

    // Teapot reflection
    m_EnvironmentShader->Bind();
    m_EnvironmentShader->SetMat4("u_Model", glm::mat4(1.f));
    virtual_pith = -m_EditorCamera.GetPitch();
    virtual_yaw = m_EditorCamera.GetYaw();
    glm::vec3 virtual_position = m_EditorCamera.GetPosition();
    virtual_position.y = -virtual_position.y;
    auto P = glm::normalize(glm::vec4(0, 1, 0, 0));
    auto reflection_mat = glm::mat4{
        -2 * P.x * P.x + 1, -2 * P.y * P.x, -2 * P.z * P.x, 0, -2 * P.x * P.y, -2 * P.y * P.y + 1, -2 * P.z * P.y, 0,
        -2 * P.x * P.z, -2 * P.y * P.z, -2 * P.z * P.z + 1, 0, -2 * P.x * P.w, -2 * P.y * P.w, -2 * P.z * P.w, 1
    };
    auto virtual_view = reflection_mat * m_EditorCamera.GetViewMatrix();
    // Model & view
    m_EnvironmentShader->SetMat4("u_ViewProjection", m_EditorCamera.GetProjection() * virtual_view);
    m_EnvironmentShader->SetFloat3("u_VirtualCameraPos", virtual_position);
    m_EnvironmentShader->SetInt("u_Skybox", 1);
    m_EnvironmentShader->SetFloat("material.shininess", m_Material.m_Shininess);
    m_EnvironmentShader->SetFloat3("material.ambient", m_Material.m_Ambient);
    m_EnvironmentShader->SetFloat3("material.diffuse", m_Material.m_Diffuse);
    m_EnvironmentShader->SetFloat3("material.specular", m_Material.m_Specular);
    m_EnvironmentShader->SetFloat3("light.position", m_Light.m_Position);
    m_EnvironmentShader->SetFloat3("light.ambient", m_Light.m_Ambient);
    m_EnvironmentShader->SetFloat3("light.diffuse", m_Light.m_Diffuse);
    m_EnvironmentShader->SetFloat3("light.specular", m_Light.m_Specular);
    m_EnvironmentShader->SetFloat("light.constant", m_Light.m_Constant);
    m_EnvironmentShader->SetFloat("light.linear", m_Light.m_Linear);
    m_EnvironmentShader->SetFloat("light.quadratic", m_Light.m_Quadratic);
    m_Teapot->Draw(*m_EnvironmentShader);
    m_Framebuffer->Unbind();
    RenderCommand::SetClearColor({0.4f, 0.4f, 0.4f, 1});
    RenderCommand::Clear();
    // Teapot
    m_EnvironmentShader->Bind();
    m_EnvironmentShader->SetFloat("material.shininess", m_Material.m_Shininess);
    m_EnvironmentShader->SetFloat3("material.ambient", m_Material.m_Ambient);
    m_EnvironmentShader->SetFloat3("material.diffuse", m_Material.m_Diffuse);
    m_EnvironmentShader->SetFloat3("material.specular", m_Material.m_Specular);
    m_EnvironmentShader->SetFloat3("light.position", m_Light.m_Position);
    m_EnvironmentShader->SetFloat3("light.ambient", m_Light.m_Ambient);
    m_EnvironmentShader->SetFloat3("light.diffuse", m_Light.m_Diffuse);
    m_EnvironmentShader->SetFloat3("light.specular", m_Light.m_Specular);
    m_EnvironmentShader->SetFloat("light.constant", m_Light.m_Constant);
    m_EnvironmentShader->SetFloat("light.linear", m_Light.m_Linear);
    m_EnvironmentShader->SetFloat("light.quadratic", m_Light.m_Quadratic);


    m_EnvironmentShader->SetMat4("u_Model", glm::mat4(1.f));
    m_EnvironmentShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
    m_EnvironmentShader->SetFloat3("u_CameraPos", m_EditorCamera.GetPosition());
    m_EnvironmentShader->SetInt("u_Skybox", 1);
    m_Teapot->Draw(*m_EnvironmentShader);


    glBindTextureUnit(2, m_Framebuffer->GetColorAttachmentRendererID());
    // Plane
    m_PlaneShader->Bind();
    m_PlaneShader->SetMat4("u_Model", glm::mat4(1.f));
    m_PlaneShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
    m_PlaneShader->SetFloat3("u_CameraPos", m_EditorCamera.GetPosition());
    m_PlaneShader->SetInt("u_Skybox", 1);
    m_PlaneShader->SetInt("u_ReflectionTexture", 2);
    m_Plane->Draw(*m_PlaneShader);


    //Point Light
    m_LightSphereShader->Bind();
    m_LightSphereShader->SetFloat3("light.ambient", m_Light.m_Ambient);
    m_LightSphereShader->SetFloat3("light.diffuse", m_Light.m_Diffuse);
    m_LightSphereShader->SetFloat3("light.specular", m_Light.m_Specular);
    glm::scale(glm::translate(glm::mat4(1.0f), m_Light.m_Position), glm::vec3(1.0f));
    m_LightSphereShader->SetMat4(
        "u_Model", glm::scale(glm::translate(glm::mat4(1.0f), m_Light.m_Position), glm::vec3(0.05f))
    );
    m_LightSphereShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
    m_Sphere->Draw(*m_LightSphereShader);


    RenderCommand::Cull(0);

    RenderCommand::DepthFunc(DepthComp::LEQUAL);
    glBindTextureUnit(1, m_CubeMap);
    m_SkyboxShader->Bind();
    m_SkyboxShader->SetInt("skybox", 1);
    auto view = glm::mat4(glm::mat3(m_EditorCamera.GetViewMatrix()));
    auto projection = glm::mat4(m_EditorCamera.GetProjection());
    m_SkyboxShader->SetMat4("u_ViewProjection", projection * view);
    m_Cube->Draw(*m_SkyboxShader);
    RenderCommand::DepthFunc(DepthComp::LESS);
}

void EditorLayer::OnImGuiRender() {
    ImGui::Begin("Propertise");
    if (ImGui::CollapsingHeader("Light Control Properties")) {
        ImGui::DragFloat("X Position", &m_Light.m_Position.x, 0.1f, -100.0f, 100.0f, "X: %.1f");
        ImGui::DragFloat("Y Position", &m_Light.m_Position.y, 0.1f, -100.0f, 100.0f, "Y: %.1f");
        ImGui::DragFloat("Z Position", &m_Light.m_Position.z, 0.1f, -100.0f, 100.0f, "Z: %.1f");

        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_Light.m_Ambient));
        ImGui::ColorEdit3("Diffuse Color", glm::value_ptr(m_Light.m_Diffuse));
        ImGui::ColorEdit3("Specular Color", glm::value_ptr(m_Light.m_Specular));

        ImGui::SliderFloat("Constant Attenuation", &m_Light.m_Constant, 0.0f, 1.0f, "Constant = %.3f");
        ImGui::SliderFloat("Linear Attenuation", &m_Light.m_Linear, 0.0f, 0.2f, "Linear = %.3f");
        ImGui::SliderFloat("Quadratic Attenuation", &m_Light.m_Quadratic, 0.0f, 0.01f, "Quadratic = %.3f");

        ImGui::SliderFloat("Radius", &radius, 0.0f, 100.0f, "Radius = %.2f");
        ImGui::SliderFloat("Sensitivity", &m_Sensitivity, 0.01f, 1.0f, "Sensitivity = %.3f");
    }

    if (ImGui::CollapsingHeader("Material Control Properties")) {
        ImGui::SliderFloat("Shininess", &m_Material.m_Shininess, 0.1f, 128.0f, "Shininess = %.3f");
    }

    ImGui::End();
}
