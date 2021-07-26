#pragma once

#include <functional>

#include <Events/Event.h>
#include <Events/KeyEvent.h>
#include <Events/MouseEvent.h>
#include <Events/WindowEvent.h>
#include <Graphics/GraphicsContext.h>

#include <Core/NebulaCommon.h>

namespace Nebula{

    enum class WindowType {
        None,
        Windows,
        MacOS,
        Linux
    };

	//Information a generic window will need for construction
	struct WindowInfo
	{
		bool windowed = true;

		std::string Title;
		uint32 Width;
		uint32 Height;
        bool Transparent;
        bool Floating;
        bool Decorated;
        bool MousePassthrough;
        bool ShowOnTaskbar;
        bool ShowInTray;

        using EventCallbackFn = std::function<void(Event&)>;
        EventCallbackFn EventCallback;

		WindowInfo(const char* title = "Nebula Editor", uint32 width = 1920, uint32 height = 1080, bool transparent = false, bool floating = false, bool decorated = true, bool mousePassthrough = false, bool showTaskbar = true, bool showInTray = false)
			: Title(title), Width(width), Height(height), Transparent(transparent), Floating(floating), Decorated(decorated) , MousePassthrough (mousePassthrough), ShowOnTaskbar(showTaskbar), ShowInTray(showInTray) {}
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

        virtual float GetTime() = 0;

		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;

		virtual void SetWindowSize(uint32 width, uint32 height) = 0;

		virtual void SetResizeable(bool resizeable) = 0;
		
		virtual uint32* GetWidthPtr() const = 0;
		virtual uint32* GetHeightPtr() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void SetIcon(std::string filepath) = 0;

		virtual void* GetNativeWindow() = 0;
		virtual Ref<GraphicsContext> GetContext() = 0;

		virtual void SwapIO(std::string in, std::string out, std::string err) = 0;

		virtual void EnableConsole() = 0;
		virtual void DisableConsole() = 0;

		// virtual void SetView(int W, int H) = 0;
		virtual inline bool IsWindowed() = 0;

		virtual bool IsMaximized() = 0;
		virtual bool IsMinimized() = 0;
		virtual bool WasMinimized() = 0;
		
		virtual void MaximizeWindow() = 0;
        virtual void RestoreWindow() = 0;
		virtual void MinimizeWindow() = 0;

		virtual void ToggleFullscreen() = 0;

        virtual Vec2u GetMaxWindowSize() = 0;

        virtual void SetPassthrough(bool enabled) = 0;
        virtual void SetFloating(bool enabled)  = 0;
        virtual void SetTransparentFramebuffer(bool enabled) = 0;
        virtual void SetDecorated(bool enabled) = 0;

		static Ref<Window> Create(const WindowInfo& inf = WindowInfo());
        static WindowType GetWindowType();

        static void HandleError();
	};
}

