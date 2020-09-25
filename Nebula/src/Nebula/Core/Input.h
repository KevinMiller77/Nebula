#pragma once

#include <map>
#include <math/math.h>

#include <Core/PlatformInfo.h>
#include <Events/InputCodes.h>

#include <GLFW/glfw3.h>

namespace Nebula
{
    class Input
    {
        std::map<KeyCode, bool> keys;
        std::map<MouseCode, bool> mouseButtons;
        Vec2f mousePos;

    public:
        Input()
        {
            for (int i = 0; i < 0xFF; i++)
            {
                keys[(KeyCode)i] = false;
                mouseButtons[(MouseCode)i] = false;
                mousePos = Vec2f(0, 0);
            }
        }

        inline static bool IsKeyPressed(KeyCode key) {return s_Input->IsKeyPressedInt(key); }
        inline static bool IsMouseButtonPressed(MouseCode key) { return s_Input->IsMouseButtonPressedInt(key); }

        inline static void SetKeyPressed(KeyCode key) { s_Input->SetKeyPressedInt(key); }    
        inline static void SetKeyReleased(KeyCode key) { s_Input->SetKeyReleasedInt(key); }

        inline static void SetMouseButtonPressed(MouseCode key) { s_Input->SetMouseButtonPressedInt(key); }    
        inline static void SetMouseButtonReleased(MouseCode key) { s_Input->SetMouseButtonReleasedInt(key); }
        
        inline static Vec2f GetMousePos() { return s_Input->GetMousePosInt(); }
        inline static void SetMousePos(Vec2f newPos) { s_Input->SetMousePosInt(newPos); }

        bool IsKeyPressedInt(KeyCode key);
        bool IsMouseButtonPressedInt(MouseCode key);
        Vec2f GetMousePosInt();

        void SetKeyPressedInt(KeyCode key) { keys[key] = true; }    
        void SetKeyReleasedInt(KeyCode key) { keys[key] = false; }
        void SetMouseButtonPressedInt(MouseCode key) { mouseButtons[key] = true; }    
        void SetMouseButtonReleasedInt(MouseCode key) { mouseButtons[key] = false; }
        void SetMousePosInt(Vec2f newPos) { mousePos = newPos; }

        static Input* s_Input;
    };
}