#pragma once

#include <Core/Input.h>
#include <unordered_map>

namespace Nebula {

    // Map to convert all nebula KeyCodes to Mac virtual key codes

    class MacInput : public Input
    {
        public:
            MacInput() = default;

            virtual bool IsKeyPressedAsyncInt(KeyCode key) override;
            virtual bool IsMouseButtonPressedAsyncInt(MouseCode key) override;
            virtual Vec2f GetMousePosInt() override;
            virtual void SetMousePosInt(Vec2f newPos) override;

            static std::unordered_map<int, KeyCode> KeyCodeMapWinToNeb;
            static std::unordered_map<KeyCode, int> KeyCodeMapNebToWin;

            static std::unordered_map<int, MouseCode> MouseCodeMapWinToNeb;
            static std::unordered_map<MouseCode, int> MouseCodeMapNebToWin;
    };
}