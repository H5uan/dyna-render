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
    glm::vec3 m_Color = {0.5f, 0.f, 0.f};
    float m_Shininess = 32.f;
};

class EditorLayer final : public GLCore::Layer {
public:
    EditorLayer();

    explicit EditorLayer(std::string normalmap_file_path = "",
                         std::string displacement_file_path = "");

    void OnAttach() override;

    void OnDetach() override;

    void OnEvent(GLCore::Event&event) override;

    void OnUpdate(GLCore::Timestep ts) override;

    void OnImGuiRender() override;

private:
    std::string m_NormalMapFilePath, m_DisplacmentFilePath;

    GLuint m_NormalMapTex, m_HeightMapTex;

    Ref<NativeOpenGLShader> m_DepthMap = CreateRef<NativeOpenGLShader>("../../Shaders/Shadow/ShadowDepth.glsl");

    Ref<NativeOpenGLShader> m_QuadShader = CreateRef<NativeOpenGLShader>(
        "../../Shaders/NormalMapping/NormalMapping.glsl");

    Ref<NativeOpenGLShader> m_LineShader = CreateRef<NativeOpenGLShader>(
        "../../Shaders/NormalMapping/Line.glsl");

    Ref<NativeOpenGLShader> m_BasicShader = CreateRef<NativeOpenGLShader>(
        "../../Shaders/BasicShader.glsl");


    Model *m_LightModel, *m_Plane;

    Ref<Framebuffer> m_Framebuffer;
    glm::vec2 m_ViewportSize = {1280.0f, 720.0f};


    GLuint m_DepthMapTex, m_DepthMapFBO;

    SpotLight m_Light;
    MaterialControl m_Material;

    GLCore::Core::Camera::EditorCamera m_EditorCamera;

    float azimuth = 20.0f;
    float elevation = 30.0f;
    float radius = 30.0f;
    float heightScale = 0.0f;
    int outerLevel = 32;
    int innerLevel = 32;
    bool ctrlPressed = false;
    bool altPressed = false;
    bool spacePressed = false;
    bool hasDisplacement = false;
    glm::vec2 lastMousePos{};
    float m_Sensitivity = 0.03f;
    float m_Reflectivity = 0.5f;
};
