#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "GLCore/Rendering/Model.h"
#include "GLCore/Rendering/NewShader.h"
#include "GLCore/Rendering/Camera/OrbitCamera.h"
#include "GLCore/Rendering/Camera/OrthographicCamera.h"
#include "GLCore/Rendering/Camera/OrthographicCameraController.h"

struct Light {
    glm::vec3 m_Position = {10.f, 10.f, 10.f};

    float m_Constant = 1.f;
    float m_Linear = 0.f;
    float m_Quadratic = 0.f;

    glm::vec3 m_Ambient = {0.05f, 0.05f, 0.05f};
    glm::vec3 m_Diffuse = {0.8f, 0.8f, 0.8f};
    glm::vec3 m_Specular = {1.f, 1.f, 1.f};
};

struct Material {
    glm::vec3 m_Ambient;
    glm::vec3 m_Diffuse;
    glm::vec3 m_Specular;
    float m_Shininess = 32.f;
};

class FrameBufferLayer final : public GLCore::Layer {
public:
    FrameBufferLayer();

    explicit FrameBufferLayer(std::string obj_file_path);

    void OnAttach() override;

    void OnDetach() override;

    void OnEvent(GLCore::Event&event) override;

    void OnUpdate(GLCore::Timestep ts) override;

    void OnImGuiRender() override;

private:
    std::string m_ObjFilePath;

    Shader* m_BlinnPhongShader;
    Shader* m_lightCubeShader;
    Shader* m_PlaneShader;
    Model* m_Model;

    static constexpr float s_CubeVertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f
    };

    static constexpr unsigned int s_CubeIndices[] = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        4, 7, 3, 3, 0, 4,
        1, 5, 6, 6, 2, 1,
        4, 0, 1, 1, 5, 4,
        3, 2, 6, 6, 7, 3
    };

    static constexpr float s_PlaneVertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f
    };

    static constexpr unsigned int s_PlaneIndices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint lightCubeVBO, lightCubeVAO, lightCubeEBO;

    GLuint planeVBO, planeVAO, planeEBO, FBO;

    GLuint renderTexture;

    GLuint depthRBO;

    Light m_Light;
    Material m_Material;

    GLCore::Core::Camera::OrbitCamera m_SceneOrbitCamera;
    GLCore::Core::Camera::SubOrbitCamera m_SubSceneOrbitCamera;

    float azimuth = 0.0f;
    float elevation = 0.0f;
    float radius = 20.0f;
    bool ctrlPressed = false;
    bool altPressed = false;
    glm::vec2 lastMousePos;
    float m_Sensitivity = 0.1f;
    bool m_Blinn = true;
};
