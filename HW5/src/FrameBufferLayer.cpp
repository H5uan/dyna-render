#include "FrameBufferLayer.h"

#include <glfw/glfw3.h>

#include <utility>

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include "GLCore/Core/MouseButtonCodes.h"

using namespace GLCore;
using namespace GLCore::Utils;

FrameBufferLayer::FrameBufferLayer(std::string obj_file_path): m_ObjFilePath(std::move(obj_file_path)),
                                                               m_SceneOrbitCamera(45.0f, 16.0f / 9.0f, 1.0f, 1000.0f),
                                                               m_SubSceneOrbitCamera(
                                                                   45.0f, 16.0f / 9.0f, 1.0f, 1000.0f) {
}

void FrameBufferLayer::OnAttach() {
    EnableGLDebugging();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    LOG_INFO("OBJ PATH: ", m_ObjFilePath);
    LOG_INFO("  {0}", m_ObjFilePath);


    m_BlinnPhongShader = new Shader(
        "../../../HW5/assets/shaders/Blinn-Phong.vert.glsl",
        "../../../HW5/assets/shaders/Blinn-Phong.frag.glsl"
    );

    m_lightCubeShader = new Shader(
        "../../../HW5/assets/shaders/LightCube.vert.glsl",
        "../../../HW5/assets/shaders/LightCube.frag.glsl"
    );

    m_PlaneShader = new Shader(
        "../../../HW5/assets/shaders/Plane.vert.glsl",
        "../../../HW5/assets/shaders/Plane.frag.glsl"
    );

    m_Model = new Model(m_ObjFilePath);

    // light cube

    glCreateBuffers(1, &lightCubeVBO);
    glNamedBufferStorage(lightCubeVBO, sizeof(s_CubeVertices), s_CubeVertices, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &lightCubeEBO);
    glNamedBufferStorage(lightCubeEBO, sizeof(s_CubeIndices), s_CubeIndices, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &lightCubeVAO);
    glVertexArrayVertexBuffer(lightCubeVAO, 0, lightCubeVBO, 0, 3 * sizeof(float));
    glVertexArrayElementBuffer(lightCubeVAO, lightCubeEBO);

    glEnableVertexArrayAttrib(lightCubeVAO, 0);
    glVertexArrayAttribFormat(lightCubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(lightCubeVAO, 0, 0);

    // plane
    glCreateBuffers(1, &planeVBO);
    glNamedBufferStorage(planeVBO, sizeof(s_PlaneVertices), s_PlaneVertices, GL_DYNAMIC_STORAGE_BIT);

    glCreateBuffers(1, &planeEBO);
    glNamedBufferStorage(planeEBO, sizeof(s_PlaneIndices), s_PlaneIndices, GL_DYNAMIC_STORAGE_BIT);

    glCreateVertexArrays(1, &planeVAO);
    glVertexArrayVertexBuffer(planeVAO, 0, planeVBO, 0, 5 * sizeof(float));
    glVertexArrayElementBuffer(planeVAO, planeEBO);

    glEnableVertexArrayAttrib(planeVAO, 0);
    glVertexArrayAttribFormat(planeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(planeVAO, 0, 0);

    glEnableVertexArrayAttrib(planeVAO, 1);
    glVertexArrayAttribFormat(planeVAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float));
    glVertexArrayAttribBinding(planeVAO, 1, 0);

    // render texture
    glCreateTextures(GL_TEXTURE_2D, 1, &renderTexture);
    glTextureParameteri(renderTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(renderTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(renderTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(renderTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureStorage2D(renderTexture, 1,GL_RGBA8, 1280, 720);
    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
    glTextureParameterf(renderTexture, GL_TEXTURE_MAX_ANISOTROPY, maxAnisotropy);

    // FBO
    glCreateFramebuffers(1, &FBO);
    glNamedFramebufferTexture(FBO, GL_COLOR_ATTACHMENT0, renderTexture, 0);
    glCreateRenderbuffers(1, &depthRBO);
    glNamedRenderbufferStorage(depthRBO, GL_DEPTH_COMPONENT24, 1280, 720);
    glNamedFramebufferRenderbuffer(FBO, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBO);

    if (glCheckNamedFramebufferStatus(FBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOG_ERROR("Framebuffer is not complete!\n");
    }
}

void FrameBufferLayer::OnDetach() {
    delete m_Model;
    delete m_BlinnPhongShader;
    delete m_lightCubeShader;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void FrameBufferLayer::OnEvent(Event&event) {
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<KeyPressedEvent>([&](const KeyPressedEvent&e) {
        switch (e.GetKeyCode()) {
            case static_cast<int>(KeyCode::LeftAlt):
            case static_cast<int>(KeyCode::RightAlt):
                altPressed = true;
                break;
            case static_cast<int>(KeyCode::F6):
                m_BlinnPhongShader->Reload();
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
        m_SceneOrbitCamera.OnEvent(event);
    }

    if (altPressed) {
        m_SubSceneOrbitCamera.OnEvent(event);
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

void FrameBufferLayer::OnUpdate(const Timestep ts) {
    m_SceneOrbitCamera.OnUpdate(ts);
    m_SubSceneOrbitCamera.OnUpdate(ts);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glClearColor(0.f, 0.f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    m_BlinnPhongShader->Use();
    glm::vec3 center = m_Model->meshes[0].m_Center;
    const glm::mat4 model = glm::translate(glm::mat4(1.0f), -center);

    m_Light.m_Position.x = radius * cos(glm::radians(elevation)) * cos(glm::radians(azimuth));
    m_Light.m_Position.y = radius * sin(glm::radians(elevation));
    m_Light.m_Position.z = radius * cos(glm::radians(elevation)) * sin(glm::radians(azimuth));

    m_BlinnPhongShader->SetFloat("material.shininess", m_Material.m_Shininess);
    m_BlinnPhongShader->SetVec3("light.position", m_Light.m_Position);
    m_BlinnPhongShader->SetVec3("light.ambient", m_Light.m_Ambient);
    m_BlinnPhongShader->SetVec3("light.diffuse", m_Light.m_Diffuse);
    m_BlinnPhongShader->SetVec3("light.specular", m_Light.m_Specular);
    m_BlinnPhongShader->SetFloat("light.constant", m_Light.m_Constant);
    m_BlinnPhongShader->SetFloat("light.linear", m_Light.m_Linear);
    m_BlinnPhongShader->SetFloat("light.quadratic", m_Light.m_Quadratic);
    m_BlinnPhongShader->SetVec3("viewPos", m_SubSceneOrbitCamera.GetPosition());
    m_BlinnPhongShader->SetMat4("u_Model", model);
    m_BlinnPhongShader->SetMat4("u_ViewProjection", m_SubSceneOrbitCamera.GetViewProjection());

    m_Model->Draw(*m_BlinnPhongShader);


    glm::mat4 new_model = glm::translate(glm::mat4(1.0f), m_Light.m_Position);
    new_model = glm::scale(new_model, glm::vec3(1.0f));
    m_lightCubeShader->Use();
    m_lightCubeShader->SetMat4("u_Model", new_model);
    m_lightCubeShader->SetMat4("u_ViewProjection", m_SubSceneOrbitCamera.GetViewProjection());
    m_lightCubeShader->SetVec3("light.ambient", m_Light.m_Ambient);
    m_lightCubeShader->SetVec3("light.diffuse", m_Light.m_Diffuse);
    m_lightCubeShader->SetVec3("light.specular", m_Light.m_Specular);


    glBindVertexArray(lightCubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_PlaneShader->Use();
    glm::mat4 plane_model = glm::mat4(1.0f);
    plane_model = glm::scale(plane_model, glm::vec3(20.f, 20.f, 20.f));
    m_PlaneShader->SetMat4("u_ViewProjection", m_SceneOrbitCamera.GetViewProjection());
    m_PlaneShader->SetMat4("u_Model", plane_model);

    glBindTextureUnit(0, renderTexture);

    glBindVertexArray(planeVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void FrameBufferLayer::OnImGuiRender() {
    ImGui::Begin("Properties");

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

    if (ImGui::CollapsingHeader("Camera & Rendering Settings")) {
        if (ImGui::Button("Reload Shader")) {
            m_BlinnPhongShader->Reload();
        }
    }


    ImGui::End();
}
