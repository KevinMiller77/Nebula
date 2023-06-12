#pragma once

#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <math/Math.h>

#include <Core/Window.h>
#include <Core/Input.h>
#include <Graphics/GraphicsContext.h>
#include <Core/NebulaCommon.h>

#include <MetalKit/MetalKit.hpp>
#include <AppKit/AppKit.hpp>

namespace Nebula{

    struct MacData
    {
        uint32 width, height;
        uint32 posX, posY;
        uint32 w_width, w_height;
        uint32 windowed_x, windowed_y;
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
        bool showOnDock = true;

        std::string title = "Nebula Studio";

        using EventCallbackFn = std::function<void(Event&)>;
        EventCallbackFn EventCallback;
    };

    class NebulaViewDelegate: public MTK::ViewDelegate {
    public:
        NebulaViewDelegate() {};
        virtual ~NebulaViewDelegate() override {};
        virtual void drawInMTKView( MTK::View* pView ) override {};
    };

    class MacWindow : public Window, public NS::ApplicationDelegate
    {
    public:
        inline bool IsWindowed() override { return !m_Data.fullscreen; }
        void ToggleFullscreen() override;
        
        MacWindow(WindowInfo info);
        ~MacWindow();
        
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

        inline virtual bool IsMaximized()  override { return m_Data.maximized; }
        inline virtual bool IsMinimized()  override { return m_Data.minimized; }
        inline virtual bool WasMinimized() override { return m_Data.wasMinimized; }

        virtual void SetPassthrough(bool enabled) override;
        virtual void SetFloating(bool enabled) override;
        virtual void SetTransparentFramebuffer(bool enabled) override;
        virtual void SetDecorated(bool enabled) override;

        // Window attributes
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; m_Data.callbackSet = true; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() override;
        virtual Ref<GraphicsContext> GetContext() override { return m_Context; }

        virtual void SwapIO(std::string in, std::string out, std::string err) override;

        virtual void EnableConsole()  override;
        virtual void DisableConsole() override;

        void ShutDown();

        static MacData m_Data; 

        void WindowDidCreateCB(NS::Window* window);

        /*
            AppDelegate functionality
        */

    private:
        void applicationWillFinishLaunching( NS::Notification* pNotification ) override;
        void applicationDidFinishLaunching( NS::Notification* pNotification ) override;
        bool applicationShouldTerminateAfterLastWindowClosed( NS::Application* pSender ) override;

        NS::Event* HandleEvent_Int(NS::Event* event);
        void CreateWindow_Int();

        NS::Menu* CreateMenuBar();

        Ref<GraphicsContext> m_Context;

        float m_FrameTime;

        NS::Application* m_Application;
        NS::Window* m_Window;

        MTK::View* m_ContentView;
        MTK::ViewDelegate* m_ViewDelegate;
        
        NS::AutoreleasePool* m_Pool;
    };
}
