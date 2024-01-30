#include "glpch.h"
#include "OrthographicCameraController.h"

#include "GLCore/Core/Input.h"
#include "GLCore/Core/KeyCodes.h"

namespace GLCore::Core::Camera {
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation, bool mouse_control)
        : aspect_ratio_(aspectRatio),
          m_Camera(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_),
          rotation_(rotation), mouse_control_(mouse_control) {
    }

    void OrthographicCameraController::OnUpdate(Timestep ts) {
        if (Input::IsKeyPressed(static_cast<int>(KeyCode::A))) {
            camera_position_.x -= cos(glm::radians(camera_rotation_)) * camera_translation_speed_ * ts;
            camera_position_.y -= sin(glm::radians(camera_rotation_)) * camera_translation_speed_ * ts;
        }
        else if (Input::IsKeyPressed(static_cast<int>(KeyCode::D))) {
            camera_position_.x += cos(glm::radians(camera_rotation_)) * camera_translation_speed_ * ts;
            camera_position_.y += sin(glm::radians(camera_rotation_)) * camera_translation_speed_ * ts;
        }

        if (Input::IsKeyPressed(static_cast<int>(KeyCode::W))) {
            camera_position_.x += -sin(glm::radians(camera_rotation_)) * camera_translation_speed_ * ts;
            camera_position_.y += cos(glm::radians(camera_rotation_)) * camera_translation_speed_ * ts;
        }
        else if (Input::IsKeyPressed(static_cast<int>(KeyCode::S))) {
            camera_position_.x -= -sin(glm::radians(camera_rotation_)) * camera_translation_speed_ * ts;
            camera_position_.y -= cos(glm::radians(camera_rotation_)) * camera_translation_speed_ * ts;
        }

        if (mouse_control_) {
            auto [mouse_x ,mouse_y] = Input::GetMousePosition();

            if (Input::IsMouseButtonPressed(static_cast<int>(KeyCode::Left))) {
                float dx = mouse_x;
            }
        }

        if (rotation_) {
            if (Input::IsKeyPressed(static_cast<int>(KeyCode::Q)))
                camera_rotation_ += camera_rotation_speed_ * ts;
            if (Input::IsKeyPressed(static_cast<int>(KeyCode::E)))
                camera_rotation_ -= camera_rotation_speed_ * ts;

            if (camera_rotation_ > 180.0f)
                camera_rotation_ -= 360.0f;
            else if (camera_rotation_ <= -180.0f)
                camera_rotation_ += 360.0f;

            m_Camera.SetRotation(camera_rotation_);
        }

        m_Camera.SetPosition(camera_position_);

        camera_translation_speed_ = zoom_level_;
    }

    void OrthographicCameraController::OnEvent(Event&e) {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(GLCORE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(GLCORE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent&e) {
        zoom_level_ -= e.GetYOffset() * 0.25f;
        zoom_level_ = std::max(zoom_level_, 0.25f);
        m_Camera.SetProjection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent&e) {
        aspect_ratio_ = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
        m_Camera.SetProjection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_, -zoom_level_, zoom_level_);
        return false;
    }
}
