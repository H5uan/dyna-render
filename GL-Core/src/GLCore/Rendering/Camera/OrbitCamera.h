#pragma once

#include "Camera.h"

#include <glm/glm.hpp>

#include "GLCore/Core/Timestep.h"
#include "GLCore/Events/MouseEvent.h"
#include "GLCore/Events/Event.h"


namespace GLCore::Core::Camera {
    class OrbitCamera : public Camera {
    public:
        OrbitCamera() = default;

        OrbitCamera(float fov, float aspectRatio, float nearClip, float farClip);

        virtual void OnUpdate(Timestep ts);

        void OnEvent(Event&e);

        [[nodiscard]] float GetDistance() const { return m_Distance; }
        void SetDistance(const float distance) { m_Distance = distance; }

        void SetViewportSize(const float width, const float height) {
            m_ViewportWidth = width;
            m_ViewportHeight = height;
            UpdateProjection();
        }

        [[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        [[nodiscard]] glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

        [[nodiscard]] glm::vec3 GetUpDirection() const;

        [[nodiscard]] glm::vec3 GetRightDirection() const;

        [[nodiscard]] glm::vec3 GetForwardDirection() const;

        [[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }

        [[nodiscard]] glm::quat GetOrientation() const;

        [[nodiscard]] float GetPitch() const { return m_Pitch; }
        [[nodiscard]] float GetYaw() const { return m_Yaw; }

    protected:
        void UpdateProjection();

        void UpdateView();

        bool OnMouseScroll(GLCore::MouseScrolledEvent&e);

        void MousePan(const glm::vec2&delta);

        void MouseRotate(const glm::vec2&delta);

        void MouseZoom(float delta);

        [[nodiscard]] glm::vec3 CalculatePosition() const;

        [[nodiscard]] std::pair<float, float> PanSpeed() const;

        [[nodiscard]] float RotationSpeed() const;

        [[nodiscard]] float ZoomSpeed() const;

    protected:
        float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

        glm::mat4 m_ViewMatrix;
        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        glm::vec3 m_FocalPoint = {0.0f, 0.0f, 0.0f};

        glm::vec2 m_InitialMousePosition = {0.0f, 0.0f};

        float m_Distance = 20.0f;
        float m_Pitch = 0.0f, m_Yaw = 0.0f;

        float m_ViewportWidth = 1280, m_ViewportHeight = 720;
    };

    class SubOrbitCamera : public OrbitCamera {
    public:
        using OrbitCamera::OrbitCamera;

        void OnUpdate(Timestep ts) override;
    };
}
