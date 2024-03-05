#pragma once

#include "GLCore/Core/Layer.h"

#include "GLCore/Events/ApplicationEvent.h"
#include "GLCore/Events/KeyEvent.h"
#include "GLCore/Events/MouseEvent.h"

namespace GLCore {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();

        ~ImGuiLayer() override = default;

        void OnAttach() override;

        void OnDetach() override;

        void Begin();

        void End();

        void OnEvent(Event&event) override;


    private:
        float m_Time = 0.0f;
        bool m_BlockEvents = true;
    };
}
