#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "GLCore/Core/Base.h"
#include "GLCore/Rendering/Model.h"
#include "GLCore/Rendering/Framebuffer.h"
#include "GLCore/Rendering/Camera/EditorCamera.h"
#include "Platform/OpenGL/NativeOpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"


struct LightControl {
    glm::vec3 m_Position = {10.f, 30.f, 10.f};

    float m_Constant = 1.f;
    float m_Linear = 0.f;
    float m_Quadratic = 0.f;

    glm::vec3 m_Ambient = {0.05f, 0.05f, 0.05f};
    glm::vec3 m_Diffuse = {0.8f, 0.8f, 0.8f};
    glm::vec3 m_Specular = {1.f, 1.f, 1.f};
};

struct MaterialControl {
    glm::vec3 m_Ambient = {0.5, 0.5, 0.5};
    glm::vec3 m_Diffuse = {1.0, 0.5, 0.31};
    glm::vec3 m_Specular = {0.5, 0.5, 0.5};
    float m_Shininess = 32.f;
};

class EditorLayer final : public GLCore::Layer {
public:
    EditorLayer();

    explicit EditorLayer(std::string obj_file_path);

    void OnAttach() override;

    void OnDetach() override;

    void OnEvent(GLCore::Event&event) override;

    void OnUpdate(GLCore::Timestep ts) override;

    void OnImGuiRender() override;

private:
    std::string m_ObjFilePath;


    Ref<NativeOpenGLShader> m_SkyboxShader = CreateRef<NativeOpenGLShader>(
        "../../../Shaders/SkyBox.glsl");

    Ref<NativeOpenGLShader> m_EnvironmentShader = CreateRef<NativeOpenGLShader>(
        "../../../Shaders/IBL/environment-mapping.glsl");

    Ref<NativeOpenGLShader> m_PlaneShader = CreateRef<NativeOpenGLShader>(
        "../../../Shaders/MirrorPlane.glsl");

    Ref<NativeOpenGLShader> m_LightSphereShader = CreateRef<NativeOpenGLShader>(
        "../../../Shaders/Light.glsl");

    GLuint m_CubeMap;
    Model *m_Cube, *m_Plane, *m_Teapot, *m_Sphere;
    /***
    std::unique_ptr<Mesh> m_Cube = std::make_unique<Mesh>(
        "Assets/Models/cube.obj", m_SkyboxShader);
    std::unique_ptr<Mesh> m_Teapot = std::make_unique<Mesh>(
        "Assets/Models/teapot.obj", m_EnvironmentShader);
    std::unique_ptr<Mesh> m_Plane = std::make_unique<Mesh>(
        "Assets/Models/plane.obj", m_PlaneShader);
***/
    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize = {1280.0f, 720.0f};


    GLuint fbo{}, textureColorBuffer{}, depthRBO{};

    LightControl m_Light;
    MaterialControl m_Material;

    float virtual_pith;
    float virtual_yaw;

    GLCore::Core::Camera::EditorCamera m_EditorCamera;

    float azimuth = 0.0f;
    float elevation = 30.0f;
    float radius = 20.0f;
    bool ctrlPressed = false;
    bool altPressed = false;
    glm::vec2 lastMousePos{};
    float m_Sensitivity = 0.1f;
    float m_Reflectivity = 0.5f;
};
