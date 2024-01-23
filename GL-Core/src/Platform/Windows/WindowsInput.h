#pragma once

#include "GLCore/Core/Input.h"

namespace GLCore {
    class WindowsInput final : public Input {
    protected:
        bool IsKeyPressedImpl(int keycode) override;

        bool IsMouseButtonPressedImpl(int button) override;

        std::pair<float, float> GetMousePositionImpl() override;

        float GetMouseXImpl() override;

        float GetMouseYImpl() override;
    };
}
