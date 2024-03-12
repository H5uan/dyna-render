#include "Light.h"

#include <glm/gtx/quaternion.hpp>

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"
#include "GLCore/Core/MouseButtonCodes.h"

using namespace GLCore;

SpotLight::SpotLight(float fov, float aspectRatio, float nearClip, float farClip)
    : mFOV(fov), mAspectRatio(aspectRatio), mNearClip(nearClip), mFarClip(farClip),
      mProjection(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip)) {
    UpdateView();
}

void SpotLight::SetCenter(const glm::vec3&center) {
    mFocalPoint = center;
    mDistance = 10.0f;
    UpdateView();
}

void SpotLight::UpdateProjection() {
    mAspectRatio = mViewportWidth / mViewportHeight;
    mProjection = glm::perspective(glm::radians(mFOV), mAspectRatio, mNearClip, mFarClip);
}

void SpotLight::UpdateView() {
    mPosition = CalculatePosition();

    glm::quat orientation = GetOrientation();
    mViewMatrix = glm::translate(glm::mat4(1.0f), mPosition) * glm::toMat4(orientation); // Model & view
    mViewMatrix = glm::inverse(mViewMatrix);
}


float SpotLight::RotationSpeed() {
    return 0.8f;
}


void SpotLight::OnUpdate(Timestep ts) {
    if (Input::IsKeyPressed(static_cast<int>(KeyCode::LeftControl)) || Input::IsKeyPressed(
            static_cast<int>(KeyCode::RightControl))) {
        const glm::vec2&mouse{Input::GetMouseX(), Input::GetMouseY()};
        glm::vec2 delta = (mouse - mInitialMousePosition) * 0.003f;
        mInitialMousePosition = mouse;

        if (Input::IsMouseButtonPressed(static_cast<int>(MouseButton::Left)))
            MouseRotate(delta);
    }

    UpdateView();
}

void SpotLight::OnEvent(Event&e) {
    EventDispatcher dispatcher(e);
}

void SpotLight::MouseRotate(const glm::vec2&delta) {
    float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
    mYaw += yawSign * delta.x * RotationSpeed();
    mPitch += delta.y * RotationSpeed();

    // clamp into limits
    mYaw = fmod(mYaw, 2.0f * glm::pi<float>());
    if (mYaw < 0) mYaw += 2.0f * glm::pi<float>();

    float maxPitch = glm::pi<float>() / 2 - glm::radians(1.0f);
    mPitch = glm::clamp(mPitch, 0.f, maxPitch);
}

glm::vec3 SpotLight::GetUpDirection() const {
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 SpotLight::GetRightDirection() const {
    return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
}

glm::vec3 SpotLight::GetForwardDirection() const {
    return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
}

glm::vec3 SpotLight::CalculatePosition() const {
    return mFocalPoint - GetForwardDirection() * mDistance;
}

glm::quat SpotLight::GetOrientation() const {
    return glm::quat(glm::vec3(-mPitch, -mYaw, 0.0f));
}
