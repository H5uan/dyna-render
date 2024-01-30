#pragma once

#include "OrthographicCamera.h"
#include "GLCore/Core/Timestep.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/MouseEvent.h"

namespace GLCore::Core::Camera {
    class OrthographicCameraController {
    public:
        explicit OrthographicCameraController(float aspectRatio, bool rotation = false, bool mouse_contorl_ = false);

        void OnUpdate(Timestep ts);

        void OnEvent(Event&e);

        OrthographicCamera& GetCamera() { return m_Camera; }
        [[nodiscard]] const OrthographicCamera& GetCamera() const { return m_Camera; }

        [[nodiscard]] float GetZoomLevel() const { return zoom_level_; }
        void SetZoomLevel(float level) { zoom_level_ = level; }

    private:
        bool OnMouseScrolled(MouseScrolledEvent&e);

        bool OnWindowResized(WindowResizeEvent&e);

    private:
        float aspect_ratio_;
        float zoom_level_ = 1.0f;
        OrthographicCamera m_Camera;

        bool rotation_, mouse_control_;

        glm::vec3 camera_position_ = {0.0f, 0.0f, 0.0f};
        float camera_rotation_ = 0.0f; //In degrees, in the anti-clockwise direction
        float camera_translation_speed_ = 5.0f, camera_rotation_speed_ = 180.0f;
    };
}
