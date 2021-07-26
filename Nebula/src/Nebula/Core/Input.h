#pragma once

#include <map>
#include <Math/math.h>

#include <Core/PlatformInfo.h>
#include <Core/Ref.h>

#include <Events/InputCodes.h>

namespace Nebula{
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

        static void Init();       

        inline static bool IsKeyPressedAsync(KeyCode key) {return s_Input->IsKeyPressedAsyncInt(key); }
        inline static bool IsMouseButtonPressedAsync(MouseCode key) { return s_Input->IsMouseButtonPressedAsyncInt(key); }

        inline static bool IsKeyPressed(KeyCode key) {return s_Input->IsKeyPressedInt(key); }
        inline static bool IsMouseButtonPressed(MouseCode key) { return s_Input->IsMouseButtonPressedInt(key); }

        inline static void SetKeyPressed(KeyCode key) { s_Input->SetKeyPressedInt(key); }    
        inline static void SetKeyReleased(KeyCode key) { s_Input->SetKeyReleasedInt(key); }

        inline static void SetMouseButtonPressed(MouseCode key) { s_Input->SetMouseButtonPressedInt(key); }    
        inline static void SetMouseButtonReleased(MouseCode key) { s_Input->SetMouseButtonReleasedInt(key); }
        
        inline static Vec2f GetMousePos() { return s_Input->GetMousePosInt(); }
        inline static void SetMousePos(Vec2f newPos) { s_Input->SetMousePosInt(newPos); }


        void SetKeyPressedInt(KeyCode key)              { keys[key] = true; }    
        void SetKeyReleasedInt(KeyCode key)             { keys[key] = false; }
        void SetMouseButtonPressedInt(MouseCode key)    { mouseButtons[key] = true; }    
        void SetMouseButtonReleasedInt(MouseCode key)   { mouseButtons[key] = false; }

        bool IsKeyPressedInt(KeyCode key)               { return keys[key]; }
        bool IsMouseButtonPressedInt(MouseCode key)     { return mouseButtons[key]; }

    protected:
        virtual bool IsKeyPressedAsyncInt(KeyCode key) = 0;
        virtual bool IsMouseButtonPressedAsyncInt(MouseCode key) = 0;
        virtual Vec2f GetMousePosInt() = 0;
        virtual void SetMousePosInt(Vec2f newPos) = 0;

    private:


        static Ref<Input> s_Input;

    };
}
