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

EditorLayer::EditorLayer(std::string normalmap_file_path,
                         std::string displacement_file_path): m_NormalMapFilePath(std::move(normalmap_file_path)),
                                                              m_DisplacmentFilePath(std::move(displacement_file_path)),
                                                              m_Light(60.f, 16.0f / 9.0f, 0.5f, 40.f),
                                                              m_EditorCamera(
                                                                  45.0f, 16.0f / 9.0f, 0.1f, 300.0f) {
}

void EditorLayer::OnAttach() {
    FramebufferSpecification fbSpec;
    fbSpec.Attachments = {
        FramebufferTextureFormat::Depth
    };
    fbSpec.Width = 1280;
    fbSpec.Height = 720;
    fbSpec.Samples = false;
    m_Framebuffer = Framebuffer::Create(fbSpec);

    glGenTextures(1, &m_NormalMapTex);
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    stbi_uc* data = stbi_load("../../../Assets/Textures/teapot_normal.png", &width, &height, &channels, 0);

    glBindTexture(GL_TEXTURE_2D, m_NormalMapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


    glGenTextures(1, &m_HeightMapTex);
    //int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    data = stbi_load("../../../Assets/Textures/teapot_disp.png", &width, &height, &channels, 0);

    glBindTexture(GL_TEXTURE_2D, m_HeightMapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    m_Plane = new Model("../../../Assets/Models/plane.glb");
    m_LightModel = new Model("../../../Assets/Models/SpotLight/light.obj");

    glGenTextures(1, &m_DepthMapTex);
    glBindTexture(GL_TEXTURE_2D, m_DepthMapTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Light.GetViewportSize().first,
                 m_Light.GetViewportSize().second, 0, GL_DEPTH_COMPONENT,
                 GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glGenFramebuffers(1, &m_DepthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMapTex, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
            case static_cast<int>(KeyCode::Space):
                spacePressed = !spacePressed;
                break;
        }
        return false;
    });

    dispatcher.Dispatch<KeyPressedEvent>([&](const KeyPressedEvent&e) {
        if (e.GetKeyCode() == static_cast<int>(KeyCode::Right)) {
            outerLevel = std::min(outerLevel + 1, 64);
            innerLevel = std::min(innerLevel + 1, 64);
        }
        else if (e.GetKeyCode() == static_cast<int>(KeyCode::Left)) {
            outerLevel = std::max(outerLevel - 1, 4);
            innerLevel = std::max(innerLevel - 1, 4);
        }
        else if (e.GetKeyCode() == static_cast<int>(KeyCode::Up)) {
            heightScale = std::min(heightScale + 0.01f, 1.0f);
        }
        else if (e.GetKeyCode() == static_cast<int>(KeyCode::Down)) {
            heightScale = std::max(heightScale - 0.01f, 0.0f);
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
            case static_cast<int>(KeyCode::Space):
                //spacePressed = false;
                break;
        }
        return false;
    });


    if (!altPressed) {
        m_EditorCamera.OnEvent(event);
    }
}

void EditorLayer::OnUpdate(const Timestep ts) {
    m_EditorCamera.OnUpdate(ts);
    m_Light.OnUpdate(ts);

    RenderCommand::Init();


    RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1.f});
    RenderCommand::Clear();
    glBindTextureUnit(0, m_HeightMapTex);


    glm::mat4 lightSpaceVP = m_Light.GetViewProjection();

    glViewport(0, 0, m_Light.GetViewportSize().first, m_Light.GetViewportSize().second);
    m_DepthMap->Bind();
    m_DepthMap->SetMat4("u_Model", glm::mat4(1.f));
    m_DepthMap->SetMat4("u_ViewProjection", lightSpaceVP);

    m_DepthMap->SetInt("displacementMap", 0);
    m_DepthMap->SetFloat("displacementScale", heightScale);

    m_Framebuffer->Bind();
    RenderCommand::Clear();
    m_Plane->Draw(*m_DepthMap, false, true, outerLevel, innerLevel);
    m_Framebuffer->Unbind();
    m_DepthMap->Unbind();
    glBindTextureUnit(1, m_Framebuffer->GetDepthAttachmentRendererID());

    glViewport(0, 0, m_ViewportSize.x, m_ViewportSize.y);
    m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);
    RenderCommand::Clear();

    m_QuadShader->Bind();

    m_QuadShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
    m_QuadShader->SetMat4("u_Model", glm::mat4(1.f));


    m_QuadShader->SetInt("displacementMap", 0);
    m_QuadShader->SetFloat("displacementScale", heightScale);

    m_QuadShader->SetFloat3("lightPos", m_Light.GetPosition());
    m_QuadShader->SetFloat3("viewPos", m_EditorCamera.GetPosition());
    m_QuadShader->SetFloat3("lightDir", m_Light.GetForwardDirection());
    m_QuadShader->SetMat4("u_LightSpaceMVPMat", lightSpaceVP);


    m_QuadShader->SetFloat3("material.color", m_Material.m_Color);
    m_QuadShader->SetFloat("material.shininess", m_Material.m_Shininess);

    m_QuadShader->SetFloat3("spotLight.direction", m_Light.GetForwardDirection());
    m_QuadShader->SetFloat3("spotLight.position", m_Light.GetPosition());
    m_QuadShader->SetFloat("spotLight.innerCos", m_Light.innerCos);
    m_QuadShader->SetFloat("spotLight.outerCos", m_Light.outerCos);
    m_QuadShader->SetFloat("spotLight.constant", m_Light.constant);
    m_QuadShader->SetFloat("spotLight.linear", m_Light.linear);
    m_QuadShader->SetFloat("spotLight.quadratic", m_Light.quadratic);
    m_QuadShader->SetFloat3("spotLight.ambient", m_Light.ambient);
    m_QuadShader->SetFloat3("spotLight.diffuse", m_Light.diffuse);
    m_QuadShader->SetFloat3("spotLight.specular", m_Light.specular);
    glBindTextureUnit(2, m_NormalMapTex);
    m_QuadShader->SetInt("texture_normal", 2);
    m_QuadShader->SetInt("depthMap", 1);

    m_Plane->Draw(*m_QuadShader, false, true, outerLevel, innerLevel);
    m_QuadShader->Unbind();

    m_LineShader->Bind();
    m_LineShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
    m_LineShader->SetMat4("u_Model", glm::mat4(1.f));
    m_LineShader->SetInt("displacmentMap", 0);
    m_LineShader->SetFloat("displacementScale", heightScale);

    m_LineShader->SetBool("isSpacePressed", spacePressed);
    m_Plane->Draw(*m_LineShader, false, true, outerLevel, innerLevel);
    m_LineShader->Unbind();

    m_BasicShader->Bind();
    m_BasicShader->SetMat4("projectionView", m_EditorCamera.GetViewProjection());

    glm::mat4 translateMat = glm::translate(glm::mat4(1.f), m_Light.GetPosition());
    glm::vec3 defaultDirection = glm::vec3(0.f, 0.f, -1.f);
    glm::vec3 rotationAxis = glm::cross(defaultDirection, m_Light.GetForwardDirection());
    float rotationAngle = acos(
        glm::dot(defaultDirection, m_Light.GetForwardDirection()) / (
            glm::length(defaultDirection) * glm::length(m_Light.GetForwardDirection())));
    glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), rotationAngle, glm::normalize(rotationAxis));
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
    glm::mat4 modelMat = translateMat * rotationMat * scaleMat;
    m_BasicShader->SetMat4("model", modelMat);
    m_LightModel->Draw(*m_BasicShader, false, false, 0, 0);
    m_BasicShader->Unbind();
}

void EditorLayer::OnImGuiRender() {
    ImGui::Begin("Propertise");
    if (ImGui::CollapsingHeader("Light Control Properties")) {
        glm::vec3 position = m_Light.GetPosition();
        ImGui::Text("Position: (%.2f, %.2f, %.2f)", position.x, position.y, position.z);

        float distance = m_Light.GetDistance();
        if (ImGui::SliderFloat("Radius", &distance, 0.0f, 100.0f, "Radius = %.2f")) {
            m_Light.SetDistance(distance);
        }

        // Displaying and editing light's attenuation factors
        ImGui::SliderFloat("Constant Attenuation", &m_Light.constant, 0.0f, 1.0f, "Constant = %.2f");
        ImGui::SliderFloat("Linear Attenuation", &m_Light.linear, 0.0f, 1.0f, "Linear = %.2f");
        ImGui::SliderFloat("Quadratic Attenuation", &m_Light.quadratic, 0.0f, 1.0f, "Quadratic = %.2f");

        // Sliders for ambient, diffuse, and specular properties
        ImGui::ColorEdit3("Ambient Color", &m_Light.ambient[0]);
        ImGui::ColorEdit3("Diffuse Color", &m_Light.diffuse[0]);
        ImGui::ColorEdit3("Specular Color", &m_Light.specular[0]);
    }

    if (ImGui::CollapsingHeader("Material Control Properties")) {
        ImGui::SliderFloat("Shininess", &m_Material.m_Shininess, 0.1f, 128.0f, "Shininess = %.3f");
    }

    if (ImGui::CollapsingHeader("Displacement Map Properties")) {
        ImGui::SliderFloat("Height Scale", &heightScale, 0.0f, 1.0f, "Scale = %.3f");
    }

    ImGui::End();
}
