#include "MacWindow.h"


#include <stb_image/stb_image.h>

#include <Core/VFS.h>
#include <Nebula_pch.h>

#ifdef NEB_PLATFORM_MACOS
#include <chrono>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <AppKit/AppKit.hpp>
#include <MetalKit/MetalKit.hpp>

void* s_WindowHandle;

namespace Nebula{

    WindowData MacWindow::data = WindowData();

    void MacWindow::HandleError() {
        // TODO: Parse and log errors
    }

    MacWindow::~MacWindow()
    {
        ShutDown();
    }

    void* MacWindow::GetNativeWindow() {
        return s_WindowHandle;
    }
    void MacWindow::SwapIO(std::string in, std::string out, std::string err)
    {
        if (!std::filesystem::exists(in))
        {
            fclose(fopen(in.c_str(), "w"));
        }
        if (!freopen(in.c_str(), "r+", stdin))
        {
            LOG_ERR("Could not open stdin\n");
        }
        if (!freopen(out.c_str(), "w", stdout))
        {
            LOG_ERR("Could not open stdout\n");
        }
        if (!freopen(err.c_str(), "w", stderr))
        {
            LOG_ERR("Could not open stderr\n");
        }

        std::ifstream t("tmpout.txt");
        if (t.is_open())
        {
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            printf("%s", str.c_str());
        }
    }

    void MacWindow::EnableConsole()
    {
        // Todo: enable a secondary console
    }

    void MacWindow::DisableConsole()
    {
        // Todo: disable secondary console
    }

    MacWindow::MacWindow(WindowInfo inf)
        : m_Context(nullptr)
    {
        NEB_PROFILE_FUNCTION();
        data.height = inf.Height; 
        data.width = inf.Width;
        data.fullscreen = !inf.windowed;

        // Hints
        data.decorated = inf.Decorated;
        data.floating = inf.Floating;
        data.transparent = inf.Transparent;
        data.mousePassthrough = inf.MousePassthrough;


        SetEventCallback(inf.EventCallback);

        LOG_INF("Creating window\n");


        s_WindowHandle = nullptr; // TODO: Make the window

        if (!s_WindowHandle) {
            LOG_ERR("Could not create window\n");
            exit(1);
        }


        // This errors because no window is actually made
        m_Context = GraphicsContext::Create(s_WindowHandle);
        m_Context->Init();
        
        UpdateWindowAttribs();

        SetWindowSize(data.width, data.height);

        // Default to VSycnc on
        SetVSync(true);
    }

    
    bool MacWindow::SetWindowStyleVar(int style, bool enable) {
        return true;
    }

    void MacWindow::SetResizeable(bool resizeable)
    {

    }

    
    Vec2u MacWindow::GetMaxWindowSize() {
        return Vec2u(data.width, data.height);
    }


    void MacWindow::ToggleFullscreen()
    {    

    }

    void MacWindow::SetPassthrough(bool enabled) {
        data.mousePassthrough = enabled;
    }

    void MacWindow::SetFloating(bool enabled) {
        data.floating = enabled;
    }

    void MacWindow::SetTransparentFramebuffer(bool enabled) {
        data.transparent = enabled;
    }

    void MacWindow::SetDecorated(bool enabled) {
        data.decorated = enabled;
    }

    std::chrono::steady_clock::time_point currentFrameTime;
    std::chrono::steady_clock::time_point lastFrameTime;
    void MacWindow::OnUpdate()
    {
        fflush(stdout);
        fflush(stderr);

        NEB_PROFILE_FUNCTION();
        if (!data.fullscreen)
        {
            data.w_width = data.width;
            data.w_height = data.height;
        }
        
        if (data.width == 0 || data.height == 0)
        {
            data.minimized = true;
        }
        else if (data.minimized)
        {
            data.minimized = false;
            data.wasMinimized = true; 
        }
        else
        {
            data.wasMinimized = false;
        }

        // MSG msg;
        // while (PeekMessage(&msg, s_WindowHandle, NULL, NULL, PM_REMOVE) > 0) {
        //     TranslateMessage(&msg);
        //     DispatchMessage(&msg);
        // }
        m_Context->SwapBuffers();

        currentFrameTime = std::chrono::high_resolution_clock::now();
        float diff = (float)std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;
        
        frameTime = diff / 1000.0f;
    }

    
    float MacWindow::GetTime() {
        return frameTime;
    }

    uint32 MacWindow::GetWidth() const
    {
        return data.width;
    }


    uint32 MacWindow::GetHeight() const
    {
        return data.height;
    }

    uint32* MacWindow::GetWidthPtr() const 
    {
        return &(data.width);
    }
    uint32* MacWindow::GetHeightPtr() const
    {
        return &(data.height);
    }

    
    void MacWindow::SetWindowSize(uint32 width, uint32 height)
    {
        // LPRECT rect = (LPRECT)malloc(sizeof(RECT));
        // if (!GetWindowRect(s_WindowHandle, rect)) {
        //     HandleError();
        //     return;
        // }

        // data.posX = rect->left;
        // data.posY = rect->top;

        // HWND windowTopOrTopmost = data.floating ? HWND_TOPMOST : HWND_TOP;
        // bool err = !SetWindowPos(s_WindowHandle,  windowTopOrTopmost,
        //     data.posX, data.posY, width, height,
        //     SWP_FRAMECHANGED | SWP_SHOWWINDOW);
            
        // if (err) {
        //     HandleError();
        //     return;
        // }

        data.width = width;
        data.height = height;
    }

    void MacWindow::UpdateWindowAttribs() {
        SetDecorated(data.decorated);
        SetPassthrough(data.mousePassthrough);
        SetTransparentFramebuffer(data.transparent);

        // Setting win size also sets floating info
        SetWindowSize(data.width, data.height);
    }  

    void MacWindow::ShutDown()
    {
        NEB_PROFILE_FUNCTION();
        // TODO: X11 shutdown
    }

    void MacWindow::SetVSync(bool enabled)
    {
        if (enabled)
        {
            m_Context->SetVSync(1);
        }
        else
        {
            m_Context->SetVSync(1);
        }

        data.vsync = enabled;
    }

    bool MacWindow::IsVSync() const
    {
        return data.vsync;
    }

    void MacWindow::SetIcon(std::string filepath)
    {
        int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		}
        if(!data) 
        {
            LOG_ERR("Failed to load image for icon!\n");
        }
        else
        {
            LOG_INF("Loaded image for icon\n");
        }

        // Set the icon of the application in X11
    }

    void MacWindow::MaximizeWindow()
    {
        // bool err = !ShowWindow(s_WindowHandle, SW_MAXIMIZE);
        // if (err) {
        //     HandleError();
        //     return;
        // }
        data.maximized = true;
    }
    void MacWindow::RestoreWindow()
    {
        // bool err = !ShowWindow(s_WindowHandle, SW_RESTORE);
        // if (err) {
        //     HandleError();
        //     return;
        // }
        data.maximized = false;
    }

    void MacWindow::MinimizeWindow()
    {
        // bool err = !ShowWindow(s_WindowHandle, SW_MINIMIZE);
        // if (err) {
        //     HandleError();
        //     return;
        // }
        data.maximized = false;
        data.minimized = true;
    }
} 
#endif