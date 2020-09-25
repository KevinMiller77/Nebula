#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <math/math.h>

#include <Core/Window.h>
#include <Graphics/GraphicsContext.h>


struct GLFWwindow;

namespace Nebula
{

    struct WindowsData
    {
        using EventCallbackFn = std::function<void(Event&)>;
        unsigned int width, height;
        int w_width, w_height;
        int windowed_x, windowed_y;
        bool windowed;
        bool VSync;
        bool callbackSet = false;
        EventCallbackFn EventCallback;
    };

    class WindowsWindow : public Window
    {
    public:
        inline bool IsWindowed() override { return data.windowed; }
        void ToggleFullscreen() override;
        virtual void CallWindowHints() override;
        
        WindowsWindow(WindowInfo inf);
        ~WindowsWindow();
        
        void OnUpdate() override;

        unsigned int GetWidth() const override;
        unsigned int GetHeight() const override;

        
        unsigned int* GetWidthPtr() const override;
        unsigned int* GetHeightPtr() const override;

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { data.EventCallback = callback; data.callbackSet = true; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() override { return (void*)window; }
        GraphicsContext* GetContext() override { return context; }
        // void SetView(int W, int H) override { context.SetView(W, H); }


        void ShutDown();
        
        static WindowsData data; 

    private:
        GLFWwindow* window;
        GraphicsContext* context;
        unsigned int GLFWWinCount;
        WindowInfo info;

    };
}