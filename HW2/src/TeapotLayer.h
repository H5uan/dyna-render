#pragma once

#include <GLCore.h>
#include <GLCoreUtils.h>

#include "GLCore/Rendering/Model.h"
#include "GLCore/Rendering/shader.h"
#include "GLCore/Rendering/Camera/OrbitCamera.h"
#include "GLCore/Rendering/Camera/OrthographicCamera.h"
#include "GLCore/Rendering/Camera/OrthographicCameraController.h"

class TeapotLayer final : public GLCore::Layer {
public:
    TeapotLayer();

    explicit TeapotLayer(std::string obj_file_path);

    void OnAttach() override;

    void OnDetach() override;

    void OnEvent(GLCore::Event&event) override;

    void OnUpdate(GLCore::Timestep ts) override;

    void OnImGuiRender() override;

private:
    std::string m_ObjFilePath;

    Shader* m_Shader;
    Model* m_Model;

    GLCore::Core::Camera::OrbitCamera m_OrbitCamera;
    GLCore::Core::Camera::OrthographicCameraController m_OrthographicCamera;
};
