#pragma once
#include <Core/PlatformInfo.h>

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <Math/math.h>

#include <Core/Window.h>
#include <Graphics/GraphicsContext.h>
#include <Core/NebulaCommon.h>


namespace Nebula{

    struct WindowData
    {
        uint32 width, height;
        uint32 posX, posY;
        int w_width, w_height;
        int windowed_x, windowed_y;
        bool fullscreen = false;

        bool vsync = true;
        bool callbackSet = false;
        bool minimized = false;
        bool wasMinimized = false;
        bool maximized = false;
        bool resizable = true;
        bool transparent = false;
        bool floating = false;
        bool decorated = true;
        bool mousePassthrough = false;
        bool showOnTaskbar = true;

        using EventCallbackFn = std::function<void(Event&)>;
        EventCallbackFn EventCallback;
    };

    class LinuxWindow : public Window
    {
    public:
        inline bool IsWindowed() override { return !data.fullscreen; }
        void ToggleFullscreen() override;
        
        LinuxWindow(WindowInfo inf);
        ~LinuxWindow();
        
        void HandleError();

        void OnUpdate() override;
        virtual float GetTime() override;

        uint32 GetWidth() const override;
        uint32 GetHeight() const override;

        virtual void SetWindowSize(uint32 width, uint32 height) override;
		virtual void SetResizeable(bool resizeable) override;
        
        uint32* GetWidthPtr() const override;
        uint32* GetHeightPtr() const override;

		virtual void SetIcon(std::string filepath) override;

		virtual void MaximizeWindow() override;
        virtual void RestoreWindow()  override; 
		virtual void MinimizeWindow() override;

        virtual Vec2u GetMaxWindowSize() override;

        inline virtual bool IsMaximized()  override { return data.maximized; }
		inline virtual bool IsMinimized()  override { return data.minimized; }
        inline virtual bool WasMinimized() override { return data.wasMinimized; }

        virtual void SetPassthrough(bool enabled) override;
        virtual void SetFloating(bool enabled) override;
        virtual void SetTransparentFramebuffer(bool enabled) override;
        virtual void SetDecorated(bool enabled) override;

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { data.EventCallback = callback; data.callbackSet = true; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() override;
        Ref<GraphicsContext> GetContext() override { return m_Context; }

		virtual void SwapIO(std::string in, std::string out, std::string err) override;

		virtual void EnableConsole()  override;
		virtual void DisableConsole() override;

        void ShutDown();
        
        static WindowData data;

    private:
        bool SetWindowStyleVar(int style, bool enable);
        void UpdateWindowAttribs();

        Ref<GraphicsContext> m_Context;

        float frameTime = 0.0f;
    };
}