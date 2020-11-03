#pragma once
#include <Core/PlatformInfo.h>
#ifdef NEB_PLATFORM_LINUX

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <math/math.h>

#include <Core/Window.h>
#include <Graphics/GraphicsContext.h>
// #include <Core/NebulaCommon.h>


struct GLFWwindow;

namespace Nebula
{

    struct WindowsData
    {
        using EventCallbackFn = std::function<void(Event&)>;
        uint32 width, height;
        int w_width, w_height;
        int windowed_x, windowed_y;
        bool windowed;
        bool VSync;
        bool callbackSet = false;
        EventCallbackFn EventCallback;
    };

    class LinuxWindow : public Window
    {
    public:
        inline bool IsWindowed() override { return data.windowed; }
        void ToggleFullscreen() override;
        virtual void CallWindowHints() override;
        
        LinuxWindow(WindowInfo inf);
        ~LinuxWindow();
        
        void OnUpdate() override;

        uint32 GetWidth() const override;
        uint32 GetHeight() const override;

        virtual void SetWindowSize(uint32 width, uint32 height) override;
        
        uint32* GetWidthPtr() const override;
        uint32* GetHeightPtr() const override;

		virtual void SetIcon(std::string filepath) override;

        virtual bool IsMaximized();
		virtual void MaximizeWindow();
        virtual void RestoreWindow();
		virtual void MinimizeWindow();

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { data.EventCallback = callback; data.callbackSet = true; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() override { return (void*)window; }
        GraphicsContext* GetContext() override { return context; }

		virtual void SwapIO(std::string in, std::string out, std::string err);

		virtual void EnableConsole();
		virtual void DisableConsole();

        void ShutDown();
        
        static WindowsData data; 

    private:
        GLFWwindow* window;
        GraphicsContext* context;
        uint32 GLFWWinCount;
        WindowInfo info;
        bool maximized = false;
    };
}
#endif