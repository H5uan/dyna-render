#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include "Camera.h"
#include "GLCore/Core/Timestep.h"
#include "GLCore/Events/Event.h"
#include "GLCore/Events/MouseEvent.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace GLCore::Core::Camera {
    class EditorCamera : public Camera {
    public:
        EditorCamera() = default;

        EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

        void OnUpdate(Timestep ts);

        void OnEvent(Event&e);

        [[nodiscard]] float GetDistance() const { return mDistance; }
        void SetDistance(float distance) { mDistance = distance; }

        void SetViewportSize(float width, float height) {
            mViewportWidth = width;
            mViewportHeight = height;
            UpdateProjection();
        }


        [[nodiscard]] const glm::mat4& GetViewMatrix() const { return mViewMatrix; }
        [[nodiscard]] glm::mat4 GetViewProjection() const { return mProjection * mViewMatrix; }

        [[nodiscard]] glm::vec3 GetUpDirection() const;

        [[nodiscard]] glm::vec3 GetRightDirection() const;

        [[nodiscard]] glm::vec3 GetForwardDirection() const;

        [[nodiscard]] const glm::vec3& GetPosition() const { return mPosition; }
        [[nodiscard]] const glm::vec3& GetCenter() const { return mFocalPoint; }


        [[nodiscard]] glm::quat GetOrientation() const;

        [[nodiscard]] float GetPitch() const { return mPitch; }
        [[nodiscard]] float GetYaw() const { return mYaw; }

        [[nodiscard]] float GetNearPlane() const { return mNearClip; }
        [[nodiscard]] float GetFarPlane() const { return mFarClip; }

        [[nodiscard]] float GetFOV() const { return mFOV; }
        [[nodiscard]] float GetAspect() const { return mAspectRatio; }

        void SetCenter(const glm::vec3&center);

    private:
        void UpdateProjection();

        void UpdateView();

        bool OnMouseScroll(MouseScrolledEvent&e);

        void MousePan(const glm::vec2&delta);

        void MouseRotate(const glm::vec2&delta);

        void MouseZoom(float delta);

        [[nodiscard]] glm::vec3 CalculatePosition() const;

        [[nodiscard]] std::pair<float, float> PanSpeed() const;

        [[nodiscard]] static float RotationSpeed();

        [[nodiscard]] float ZoomSpeed() const;

    public:
        float mCameraSpeed = 1.0f;

    private:
        float mFOV = 45.0f, mAspectRatio = 1.778f, mNearClip = 0.1f, mFarClip = 1000.0f;

        glm::mat4 mViewMatrix;
        glm::vec3 mPosition = {0.0f, 0.0f, 0.0f};
        glm::vec3 mFocalPoint = {0.0f, 0.0f, 0.0f};

        glm::vec2 mInitialMousePosition = {0.0f, 0.0f};

        float mDistance = 10.0f;
        float mPitch =0.2f, mYaw = 0.0f;

        float mViewportWidth = 1280, mViewportHeight = 720;

        bool mInit = true;
    };
}
