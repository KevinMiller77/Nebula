#include "Window.h"

#include <Core/Input.h>

#include <Platform/OS/Windows/WindowsWindow.h>
// #include <platform/Linux/LinuxWindow.h>

namespace Nebula
{
	Window* Window::Create(const WindowInfo& inf)
	{
	#ifdef NEB_PLATFORM_WINDOWS
		return new WindowsWindow(inf);
	#endif
	#ifdef NEB_PLATFORM_LINUX
		return new LinuxWindow(inf);
	#endif
	#ifdef NEB_PLATFORM_MACOS
		return new WindowsWindow(inf);
	#endif
	}

}