#include "Application.h"
#include "Window.h"
#include "Input.h"

#include <Platform/OS/MacOS/MacInput.h>
#include <Platform/OS/Windows/WindowsInput.h>

namespace Nebula {
    
    Ref<Input> Input::s_Input = nullptr;

    Ref<Input> CreateInput() {
        switch (Window::GetWindowType()) {
            case (WindowType::None) : { LOG_ERR("No OS was given! Cannot create input subsystem\n"); return nullptr; }
            case (WindowType::Windows) : { return CreateRef<WindowsInput>(); }
            case (WindowType::MacOS): { return CreateRef<MacInput>(); }
            default: { LOG_ERR("No OS was given! Cannot create input subsystem\n"); return nullptr; }
        }
    }

    void Input::Init() {
        s_Input = CreateInput();
    }
}
