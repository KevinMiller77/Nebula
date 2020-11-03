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