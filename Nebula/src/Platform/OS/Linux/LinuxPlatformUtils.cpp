#include "Nebula.h"
#ifdef NEB_PLATFORM_LINUX

#include <GLFW/glfw3.h>

//Include the x11 native

namespace Nebula
{
    std::string FileDialogs::OpenFile(const char* filter)
    {
        // OPENFILENAMEA ofn;
		// CHAR szFile[256] = { 0 };
		// ZeroMemory(&ofn, sizeof(OPENFILENAME));
		// ofn.lStructSize = sizeof(OPENFILENAME);
		// ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get()->GetWindow()->GetNativeWindow());
		// ofn.lpstrFile = szFile;
		// ofn.nMaxFile = sizeof(szFile);
		// ofn.lpstrFilter = filter;
		// ofn.nFilterIndex = 1;
		// ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		// if (GetOpenFileNameA(&ofn) == TRUE)
		// {
		// 	return ofn.lpstrFile;
		// }
		return std::string();
    }

	std::string FileDialogs::SaveFile(const char* filter)
	{
		// OPENFILENAMEA ofn;
		// CHAR szFile[260] = { 0 };
		// ZeroMemory(&ofn, sizeof(OPENFILENAME));
		// ofn.lStructSize = sizeof(OPENFILENAME);
		// ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Application::Get()->GetWindow()->GetNativeWindow());
		// ofn.lpstrFile = szFile;
		// ofn.nMaxFile = sizeof(szFile);
		// ofn.lpstrFilter = filter;
		// ofn.nFilterIndex = 1;
		// ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		// if (GetSaveFileNameA(&ofn) == TRUE)
		// {
		// 	return ofn.lpstrFile;
		// }
		return std::string();
	}
}
#endif