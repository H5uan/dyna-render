#include <glpch.h>

#include "OrbitCamera.h"

#include <GLCore/Core/Input.h>
#include <GLCore/Core/KeyCodes.h>
#include <GLCore/Core/MouseButtonCodes.h>

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace GLCore::Core::Camera {
    OrbitCamera::OrbitCamera(float fov, float aspectRatio, float nearClip, float farClip)
        : Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)),
          m_FOV(fov), m_AspectRatio(aspectRatio),
          m_NearClip(nearClip),
          m_FarClip(farClip) {
        UpdateView();
    }

    void OrbitCamera::UpdateProjection() {
        m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
        m_Projection = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_NearClip, m_FarClip);
    }

    void OrbitCamera::UpdateView() {
        m_Position = CalculatePosition();

        glm::quat orientation = GetOrientation();
        m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix = glm::inverse(m_ViewMatrix);
    }

    std::pair<float, float> OrbitCamera::PanSpeed() const {
        float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
        float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
        float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return {xFactor, yFactor};
    }

    float OrbitCamera::RotationSpeed() const {
        return 0.5f;
    }

    float OrbitCamera::ZoomSpeed() const {
        float distance = m_Distance * 0.2f;
        distance = std::max(distance, 0.0f);
        float speed = distance * distance;
        speed = std::min(speed, 100.0f); // max speed = 100
        return speed;
    }

    void OrbitCamera::OnUpdate(Timestep ts) {
        const glm::vec2&mouse{Input::GetMouseX(), Input::GetMouseY()};
        glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
        m_InitialMousePosition = mouse;

        if (Input::IsMouseButtonPressed(static_cast<int>(MouseButton::Middle))) {
            //LOG_INFO("Mouse Button Pressed: Middle");
            MousePan(delta);
        }
        else if (Input::IsMouseButtonPressed(static_cast<int>(MouseButton::Left))) {
            //LOG_INFO("Mouse Button Pressed: Left");
            MouseRotate(delta);
        }
        else if (Input::IsMouseButtonPressed(static_cast<int>(MouseButton::Right))) {
            //LOG_INFO("Mouse Button Pressed: Right");
            MouseZoom(delta.y);
        }

        UpdateView();
    }

    void OrbitCamera::OnEvent(Event&e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(OrbitCamera::OnMouseScroll));
    }

    bool OrbitCamera::OnMouseScroll(MouseScrolledEvent&e) {
        float delta = e.GetYOffset() * 0.1f;
        MouseZoom(delta);
        UpdateView();
        return false;
    }

    void OrbitCamera::MousePan(const glm::vec2&delta) {
        auto [xSpeed, ySpeed] = PanSpeed();
        m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
        m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
    }

    void OrbitCamera::MouseRotate(const glm::vec2&delta) {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        m_Yaw += yawSign * delta.x * RotationSpeed();
        m_Pitch += delta.y * RotationSpeed();
    }

    void OrbitCamera::MouseZoom(float delta) {
        m_Distance -= delta * ZoomSpeed();
        if (m_Distance < 1.0f) {
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }

    glm::vec3 OrbitCamera::GetUpDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 OrbitCamera::GetRightDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 OrbitCamera::GetForwardDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::vec3 OrbitCamera::CalculatePosition() const {
        return m_FocalPoint - GetForwardDirection() * m_Distance;
    }

    glm::quat OrbitCamera::GetOrientation() const {
        return {glm::vec3(-m_Pitch, -m_Yaw, 0.0f)};
    }
}
