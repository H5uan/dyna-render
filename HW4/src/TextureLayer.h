#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "GLCore/Rendering/Model.h"
#include "GLCore/Rendering/shader.h"
#include "GLCore/Rendering/Camera/OrbitCamera.h"
#include "GLCore/Rendering/Camera/OrthographicCamera.h"
#include "GLCore/Rendering/Camera/OrthographicCameraController.h"

struct Light {
    glm::vec3 m_Position = {10.f, 10.f, 10.f};

    float m_Constant = 1.f;
    float m_Linear = 0.09f;
    float m_Quadratic = 0.032f;

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

class TextureLayer final : public GLCore::Layer {
public:
    TextureLayer();

    explicit TextureLayer(std::string obj_file_path);

    void OnAttach() override;

    void OnDetach() override;

    void OnEvent(GLCore::Event&event) override;

    void OnUpdate(GLCore::Timestep ts) override;

    void OnImGuiRender() override;

private:
    std::string m_ObjFilePath;

    Shader* m_BlinnPhongShader;
    Shader* m_lightCubeShader;
    Model* m_Model;

    static constexpr float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
    };

    static constexpr unsigned int indices[] = {
        0, 1, 2, 2, 3, 0,
        1, 5, 6, 6, 2, 1,
        7, 6, 5, 5, 4, 7,
        4, 0, 3, 3, 7, 4,
        4, 5, 1, 1, 0, 4,
        3, 2, 6, 6, 7, 3
    };

    GLuint VBO, lightCubeVAO, lightCubeEBO;

    Light m_Light;
    Material m_Material;

    GLCore::Core::Camera::OrbitCamera m_OrbitCamera;
    GLCore::Core::Camera::OrthographicCameraController m_OrthographicCamera;

    float azimuth = 0.0f;
    float elevation = 0.0f;
    float radius = 30.0f;
    bool ctrlPressed = false;
    glm::vec2 lastMousePos;
    float m_Sensitivity = 0.1f;
    bool m_Blinn = true;
};