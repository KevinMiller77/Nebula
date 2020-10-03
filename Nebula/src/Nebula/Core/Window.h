#pragma once

#include <functional>

#include <Events/Event.h>
#include <Events/KeyEvent.h>
#include <Events/MouseEvent.h>
#include <Events/WindowEvent.h>
#include <Graphics/GraphicsContext.h>

#include <Core/NebulaCommon.h>

namespace Nebula
{
	//Information a generic window will need for construction
	struct WindowInfo
	{
		bool windowed = true;

		const char* Title;
		uint32 Width;
		uint32 Height;

		WindowInfo(const char* title = "Nebula Editor", uint32 width = 1920, uint32 height = 1080)
			: Title(title), Width(width), Height(height) {}
	};

	//Platform independent window function.
	//This will represent the entry point to our engine
	//This class should be overridden for the platform being used
	class Window
	{
	public:
		Window() {};
		~Window() {};
		
		//TODO: Don't using a std::function
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void OnUpdate() = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		
		virtual uint32* GetWidthPtr() const = 0;
		virtual uint32* GetHeightPtr() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() = 0;
		virtual GraphicsContext* GetContext() = 0;

		// virtual void SetView(int W, int H) = 0;
		virtual inline bool IsWindowed() = 0;

		virtual void ToggleFullscreen() = 0;
		virtual void CallWindowHints() = 0;

		static Window* Create(const WindowInfo& inf = WindowInfo());
	};
}

