#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "GLCore/Core/Base.h"
#include "GLCore/Rendering/Model.h"
#include "GLCore/Rendering/Framebuffer.h"
#include "GLCore/Rendering/Light.h"
#include "GLCore/Rendering/Camera/EditorCamera.h"
#include "Platform/OpenGL/NativeOpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexture.h"


struct MaterialControl {
    glm::vec3 m_Color = {1.f, 1.f, 1.f};
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

    Ref<NativeOpenGLShader> m_DepthMap = CreateRef<NativeOpenGLShader>("../../../Shaders/Shadow/ShadowDepth.glsl");
    Ref<NativeOpenGLShader> m_ShadowMapping = CreateRef<NativeOpenGLShader>(
        "../../../Shaders/Shadow/ShadowMapping.glsl");

    Ref<NativeOpenGLShader> m_PlaneShader = CreateRef<NativeOpenGLShader>(
        "../../../Shaders/Shadow/DebugPlane.glsl");

    Ref<NativeOpenGLShader> m_BasicShader = CreateRef<NativeOpenGLShader>(
        "../../../Shaders/BasicShader.glsl");

    GLuint m_CubeMap;
    Model *m_LightModel, *m_Plane, *m_Teapot;
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


    GLuint m_DepthMapTex, m_DepthMapFBO;

    SpotLight m_Light;
    MaterialControl m_Material;


    GLCore::Core::Camera::EditorCamera m_EditorCamera;

    float azimuth = 20.0f;
    float elevation = 30.0f;
    float radius = 30.0f;
    bool ctrlPressed = false;
    bool altPressed = false;
    glm::vec2 lastMousePos{};
    float m_Sensitivity = 0.03f;
    float m_Reflectivity = 0.5f;
};
