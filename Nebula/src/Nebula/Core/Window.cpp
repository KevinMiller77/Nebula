#include "Window.h"

#include <Core/Input.h>

#ifdef NEB_PLATFORM_WINDOWS
#include <Platform/OS/Windows/WindowsWindow.h>
#endif
#ifdef NEB_PLATFORM_LINUX
#include <Platform/OS/Linux/LinuxWindow.h>
#endif
#ifdef NEB_PLATFORM_MACOS
#include <Platform/OS/MacOS/MacWindow.h>
#endif


namespace Nebula{

    WindowType Window::GetWindowType()
    {
    #ifdef NEB_PLATFORM_WINDOWS
        return WindowType::Windows;
    #endif
    #ifdef NEB_PLATFORM_LINUX
        return WindowType::MacOS;
    #endif
    #ifdef NEB_PLATFORM_MACOS
        return WindowType::Linux;
    #endif
    }

    Ref<Window> Window::Create(const WindowInfo& inf)
	{
	#ifdef NEB_PLATFORM_WINDOWS
		return CreateRef<WindowsWindow>(inf);
	#endif
	#ifdef NEB_PLATFORM_LINUX
		return CreateRef<LinuxWindow>(inf);
	#endif
	#ifdef NEB_PLATFORM_MACOS
		return CreateRef<WindowsWindow>(inf);
	#endif
	}

}