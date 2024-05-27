#pragma once

#include <Core/Input.h>
#include <unordered_map>

namespace Nebula {

    class MacInput : public Input
    {
        public:
            MacInput() = default;

            virtual bool IsKeyPressedAsyncInt(KeyCode key) override;
            virtual bool IsMouseButtonPressedAsyncInt(MouseCode key) override;
            virtual Vec2f GetMousePosInt() override;
            virtual void SetMousePosInt(Vec2f newPos) override;

            static std::unordered_map<int, KeyCode> KeyCodeMapMacToNeb;
            static std::unordered_map<KeyCode, int> KeyCodeMapNebToMac;

            static std::unordered_map<int, MouseCode> MouseCodeMapMacToNeb;
            static std::unordered_map<MouseCode, int> MouseCodeMapNebToMac;
    };
}