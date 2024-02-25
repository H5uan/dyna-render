#include "IBLLayer.h"

#include <stb_image.h>
#include <glfw/glfw3.h>

#include <utility>

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include "GLCore/Core/MouseButtonCodes.h"

using namespace GLCore;
using namespace GLCore::Utils;

IBLLayer::IBLLayer(std::string obj_file_path): m_ObjFilePath(std::move(obj_file_path)),
                                               m_EditorCamera(45.0f, 16.0f / 9.0f, 1.0f, 1000.0f) {
}

void IBLLayer::OnAttach() {
    EnableGLDebugging();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_DEPTH_TEST);

    LOG_INFO("OBJ PATH: ", m_ObjFilePath);
    LOG_INFO("  {0}", m_ObjFilePath);

    m_SkyBoxShader = new Shader(
        "../../../HW6/assets/shaders/SkyBox.vert.glsl",
        "../../../HW6/assets/shaders/SkyBox.frag.glsl"
    );

    m_PlaneShader = new Shader(
        "../../../HW6/assets/shaders/Plane.vert.glsl",
        "../../../HW6/assets/shaders/Plane.frag.glsl"
    );

    m_EnvironmentShader = new Shader(
        "../../../HW6/assets/shaders/IBL/environment-mapping.vert.glsl",
        "../../../HW6/assets/shaders/IBL/environment-mapping.frag.glsl");

    m_Model = new Model(m_ObjFilePath);

    // cube map
    textureFaces = {
        "../../../HW6/assets/models/cubemap/cubemap_posx.png",
        "../../../HW6/assets/models/cubemap/cubemap_negx.png",
        "../../../HW6/assets/models/cubemap/cubemap_posy.png",
        "../../../HW6/assets/models/cubemap/cubemap_negy.png",
        "../../../HW6/assets/models/cubemap/cubemap_posz.png",
        "../../../HW6/assets/models/cubemap/cubemap_negz.png"
    };

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &cubemapTexture);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(textureFaces[0].c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        LOG_ERROR("Cubemap texture failed to load at path: ", textureFaces[0]);
    }
    else {
        glTextureStorage2D(cubemapTexture, 1, GL_RGBA8, width, height);

        for (GLuint i = 0; i < 6; i++) {
            if (i > 0) {
                data = stbi_load(textureFaces[i].c_str(), &width, &height, &nrChannels, 0);
                if (!data) {
                    LOG_ERROR("Cubemap texture failed to load at path: ", textureFaces[i]);
                }
            }

            glTextureSubImage3D(cubemapTexture, 0, 0, 0, i, width, height, 1, GL_RGB, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
    }

    glTextureParameteri(cubemapTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(cubemapTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(cubemapTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(cubemapTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(cubemapTexture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Cube
    glCreateBuffers(1, &CubeVBO);
    glCreateBuffers(1, &CubeEBO);

    glNamedBufferData(CubeVBO, sizeof(s_CubeVertices), s_CubeVertices, GL_STATIC_DRAW);
    glNamedBufferData(CubeEBO, sizeof(s_CubeIndices), s_CubeIndices, GL_STATIC_DRAW);

    glCreateVertexArrays(1, &CubeVAO);
    glVertexArrayVertexBuffer(CubeVAO, 0, CubeVBO, 0, 3 * sizeof(float));
    glEnableVertexArrayAttrib(CubeVAO, 0);
    glVertexArrayAttribFormat(CubeVAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(CubeVAO, 0, 0);
    glVertexArrayElementBuffer(CubeVAO, CubeEBO);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);


    m_EditorCamera.SetDistance(40.F);
}

void IBLLayer::OnDetach() {
    delete m_Model;
    delete m_SkyBoxShader;

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void IBLLayer::OnEvent(Event&event) {
    EventDispatcher dispatcher(event);

    dispatcher.Dispatch<KeyPressedEvent>([&](const KeyPressedEvent&e) {
        switch (e.GetKeyCode()) {
            case static_cast<int>(KeyCode::LeftAlt):
            case static_cast<int>(KeyCode::RightAlt):
                altPressed = true;
                break;
            case static_cast<int>(KeyCode::F6):
                //m_BlinnPhongShader->Reload();
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

void IBLLayer::OnUpdate(const Timestep ts) {
    m_EditorCamera.OnUpdate(ts);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_EnvironmentShader->Use();
    auto model = glm::mat4(1.0f);
    m_EnvironmentShader->SetMat4("u_Model", model);
    m_EnvironmentShader->SetMat4("u_ViewProjection", m_EditorCamera.GetViewProjection());
    m_EnvironmentShader->SetVec3("u_CameraPos", m_EditorCamera.GetPosition());
    m_Model->Draw(*m_EnvironmentShader);

    glDepthFunc(GL_LEQUAL);
    m_SkyBoxShader->Use();
    auto view = glm::mat4(glm::mat3(m_EditorCamera.GetViewMatrix()));
    auto projection = glm::mat4(m_EditorCamera.GetProjection());
    m_SkyBoxShader->SetMat4("u_ViewProjection", projection * view);

    glBindVertexArray(CubeVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawElements(GL_TRIANGLES, std::size(s_CubeIndices), GL_UNSIGNED_INT, nullptr);

    glDepthFunc(GL_LESS);
}

void IBLLayer::OnImGuiRender() {
    ImGui::Begin("Properties");

    if (ImGui::CollapsingHeader("Camera & Rendering Settings")) {
        if (ImGui::Button("Reload Shader")) {
            //m_BlinnPhongShader->Reload();
        }

        glm::vec3 camPosition = m_EditorCamera.GetPosition();
        ImGui::Text("Camera Position: (%.2f, %.2f, %.2f)", camPosition.x, camPosition.y, camPosition.z);
    }


    ImGui::End();
}
