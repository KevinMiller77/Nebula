#include "WindowsWindow.h"
#include "WindowsInput.h"

#include <Core/PlatformInfo.h>

#ifdef NEB_PLATFORM_WINDOWS

#include <Windows.h>
#include <dwmapi.h>

#include <stb_image/stb_image.h>


#include <Core/VFS.h>
#include <Nebula_pch.h>

#include <chrono>


#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

HWND s_WindowHandle;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
const DWORD s_DefaultWindowedStyle = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN;
const DWORD s_DefaultFullscreenStyle = WS_POPUP | WS_VISIBLE;

#endif

namespace Nebula{
    #ifdef NEB_PLATFORM_WINDOWS
    WindowsData WindowsWindow::data = WindowsData();
    Ref<WindowsWindow> winRef;

    LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;
        switch (msg) {
            case WM_CLOSE: {
                WindowCloseEvent event = WindowCloseEvent();
                winRef->data.EventCallback(event);
                break;
            }
            case WM_DESTROY: {
                PostQuitMessage(0);
                break;
            }
            case WM_SYSKEYDOWN: 
            case WM_KEYDOWN: {
                // Get repeat count
                int hitCount = lParam & 0xffff;
                bool repeat = hitCount > 1;


                if (WindowsInput::KeyCodeMapWinToNeb.find(wParam) != WindowsInput::KeyCodeMapWinToNeb.end()) {
                    Input::SetKeyPressed(WindowsInput::KeyCodeMapWinToNeb[wParam]);
                    KeyPressedEvent event((int)WindowsInput::KeyCodeMapWinToNeb[wParam], hitCount - 1);
                    winRef->data.EventCallback(event);
                }
                break;
            } 
            case WM_SYSKEYUP:
            case WM_KEYUP: {
                if (WindowsInput::KeyCodeMapWinToNeb.find(wParam) != WindowsInput::KeyCodeMapWinToNeb.end()) {
                    KeyReleasedEvent event((int)WindowsInput::MouseCodeMapWinToNeb[wParam]);
                    winRef->data.EventCallback(event);
                    Input::SetKeyReleased(WindowsInput::KeyCodeMapWinToNeb[wParam]);
                }
                break;
            }
            case WM_XBUTTONDOWN: {
                MouseCode xButton = (wParam >> 16) == 0x0001 ? MouseCode::ButtonLast : MouseCode::ButtonNext;
                MouseButtonPressedEvent event(xButton);
                winRef->data.EventCallback(event);
                Input::SetMouseButtonPressed(xButton);
                break; 
            }
            case WM_LBUTTONDOWN: {
                MouseButtonPressedEvent event(MouseCode::ButtonLeft);
                winRef->data.EventCallback(event);
                Input::SetMouseButtonPressed(MouseCode::ButtonLeft);
                break;
            }
            case WM_MBUTTONDOWN: {
                MouseButtonPressedEvent event(MouseCode::ButtonMiddle);
                winRef->data.EventCallback(event);
                Input::SetMouseButtonPressed(MouseCode::ButtonMiddle);
                break;
            }
            case WM_RBUTTONDOWN: {
                MouseButtonPressedEvent event(MouseCode::ButtonRight);
                winRef->data.EventCallback(event);
                Input::SetMouseButtonPressed(MouseCode::ButtonRight);
                break;
            }

            case WM_XBUTTONUP:  {
                MouseCode xButton = (wParam >> 16) == 0x0001 ? MouseCode::ButtonLast : MouseCode::ButtonNext;
                MouseButtonReleasedEvent event(xButton);
                winRef->data.EventCallback(event);
                Input::SetMouseButtonReleased(xButton);
                break;                    
            }
            case WM_LBUTTONUP: {
                MouseButtonReleasedEvent event(MouseCode::ButtonLeft);
                winRef->data.EventCallback(event);
                Input::SetMouseButtonReleased(MouseCode::ButtonLeft);
                break;
            }
            case WM_RBUTTONUP: {
                MouseButtonReleasedEvent event(MouseCode::ButtonRight);
                winRef->data.EventCallback(event);
                Input::SetMouseButtonReleased(MouseCode::ButtonRight);
                break;
            }
            case WM_MBUTTONUP: {
                MouseButtonReleasedEvent event(MouseCode::ButtonMiddle);
                winRef->data.EventCallback(event);
                Input::SetMouseButtonReleased(MouseCode::ButtonMiddle);
                break;
            }
            
            case WM_MOUSEWHEEL: {
                int scrollDist = GET_WHEEL_DELTA_WPARAM(wParam);
                MouseScrolledEvent event({0, ((float)scrollDist / (float)WHEEL_DELTA)});
                winRef->data.EventCallback(event);
                break;
            }
            case WM_MOUSEHWHEEL: {
                int scrollDist = GET_WHEEL_DELTA_WPARAM(wParam);
                MouseScrolledEvent event({((float)scrollDist / (float)WHEEL_DELTA), 0});
                winRef->data.EventCallback(event);
                break;
            }
            case WM_SIZE: {
                WindowResizeEvent event({LOWORD(lParam), HIWORD(lParam)});
                winRef->data.EventCallback(event);
                break;
            }
            case WM_SIZING: {
                RECT* rect = (RECT*)lParam;
                uint16_t width = rect->right - rect->left;
                uint16_t height = rect->bottom - rect->top;
                WindowResizeEvent event({width, height});
                winRef->data.EventCallback(event);
                break;
            }
            case WM_MOVING: {
                LockWindowUpdate(NULL);
                break;
            }
            case WM_SETFOCUS: {
                WindowFocusEvent event = WindowFocusEvent();
                winRef->data.EventCallback(event);
                break;
            }
            case WM_KILLFOCUS: {
                WindowLostFocusEvent event = WindowLostFocusEvent();
                winRef->data.EventCallback(event);
                break;
            }
            default: 
                return DefWindowProc(hWnd, msg, wParam, lParam); 
        }
        return 0;
    }

    void HandleError_Int() {
        DWORD error = GetLastError();
        char *message = NULL;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&message,
            0,
            NULL);

        LOG_ERR_TAG("[Window]", "%s\n", message);
    }

    void WindowsWindow::HandleError() {
        HandleError_Int();
    }

    WindowsWindow::~WindowsWindow()
    {
        ShutDown();
    }

    void* WindowsWindow::GetNativeWindow() {
        return s_WindowHandle;
    }
    void WindowsWindow::SwapIO(std::string in, std::string out, std::string err)
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

    void WindowsWindow::EnableConsole()
    {
        ::ShowWindow(::GetConsoleWindow(), SW_SHOW);
    }

    void WindowsWindow::DisableConsole()
    {
        ::ShowWindow(::GetConsoleWindow(), SW_HIDE);
    }

    WindowsWindow::WindowsWindow(WindowInfo inf)
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

        LPCSTR WNDCLASS_NAME_CSTR = "NebulaWindowClass";
        const wchar_t WNDCLASS_NAME[] = L"NebulaWindowClass";

        WNDCLASSEX wc = { sizeof(WNDCLASSEX), 
                        CS_CLASSDC | CS_OWNDC, 
                        WndProc, 
                        0L, 
                        0L, 
                        GetModuleHandle(NULL), 
                        NULL, 
                        NULL, 
                        NULL, 
                        NULL, 
                        WNDCLASS_NAME, 
                        NULL };
    
        if(!::RegisterClassEx(&wc)) {
            assert((1, "Could not register window class"));
        }

        s_WindowHandle = ::CreateWindowExA( inf.ShowOnTaskbar ? 0 : WS_EX_TOOLWINDOW,
                                        WNDCLASS_NAME_CSTR, 
                                        (LPCSTR)inf.Title.c_str(), 
                                        inf.ShowOnTaskbar ? WS_OVERLAPPEDWINDOW : WS_BORDER, 
                                        0, 
                                        0, 
                                        data.width, 
                                        data.height, 
                                        NULL, 
                                        NULL, 
                                        wc.hInstance, 
                                        NULL);

        if (!s_WindowHandle) {
            LOG_ERR("Could not create window\n");
            exit(1);
        }

        m_Context = GraphicsContext::Create(s_WindowHandle);
        m_Context->Init();
        
        UpdateWindowAttribs();
        
        LockWindowUpdate(NULL);

        SetWindowSize(data.width, data.height);

        // Default to VSycnc on
        SetVSync(true);
    }

    
    bool WindowsWindow::SetWindowStyleVar(int style, bool enable) {
        DWORD dwStyle = GetWindowLong(s_WindowHandle, GWL_STYLE);
        if (enable) {
            dwStyle |= style;
        }
        else {
            dwStyle &= ~style;
        }
        return SetWindowLong(s_WindowHandle, GWL_STYLE, dwStyle) != 0;
    }

    void WindowsWindow::SetResizeable(bool resizeable)
    {
        SetWindowStyleVar(WS_MAXIMIZEBOX, resizeable);
    }

    
    Vec2u WindowsWindow::GetMaxWindowSize() {
        int windowWidth = GetDeviceCaps(GetDC(s_WindowHandle), HORZRES);
        int windowHeight = GetDeviceCaps(GetDC(s_WindowHandle), VERTRES);

        return Vec2u(windowWidth, windowHeight);
    }


    void WindowsWindow::ToggleFullscreen()
    {    
        if (!data.fullscreen) {

            // Get window position and save it to data posX and posY
            RECT rect;
            GetWindowRect(s_WindowHandle, &rect);
            data.posX = rect.left;
            data.posY = rect.top;
            data.width = rect.right - rect.left;
            data.height = rect.bottom - rect.top;

            // Get the current monitor size
            POINT Point = {0};
            HMONITOR Monitor = MonitorFromPoint(Point, MONITOR_DEFAULTTONEAREST);
            MONITORINFO MonitorInfo = { sizeof(MonitorInfo) };
            if (GetMonitorInfo(Monitor, &MonitorInfo)) {
                DWORD Style = s_DefaultFullscreenStyle;
                SetWindowLongPtr(s_WindowHandle, GWL_STYLE, Style);

                // Set the window to the monitor size
                bool err = !SetWindowPos(   s_WindowHandle, 
                                0, 
                                MonitorInfo.rcMonitor.left, 
                                MonitorInfo.rcMonitor.top,
                                MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left, 
                                MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
                                SWP_FRAMECHANGED | SWP_SHOWWINDOW
                            );

                if (err) {
                    HandleError();
                    return;
                }
                data.fullscreen = true;
            }
        } else {
            SetWindowLongPtr(s_WindowHandle, GWL_STYLE, s_DefaultWindowedStyle);
            bool err = !SetWindowPos(s_WindowHandle, HWND_TOPMOST,
                data.posX, data.posY, data.w_width, data.w_height,
                SWP_FRAMECHANGED | SWP_SHOWWINDOW);
            if (err) {
                HandleError();
                return;
            }
            data.fullscreen  = false;
        }
    }

    void WindowsWindow::SetPassthrough(bool enabled) {
        COLORREF key = 0;
        BYTE alpha = 0;
        DWORD flags = 0;
        DWORD exStyle = GetWindowLongW(s_WindowHandle, GWL_EXSTYLE);

        if (exStyle & WS_EX_LAYERED)
            GetLayeredWindowAttributes(s_WindowHandle, &key, &alpha, &flags);

        if (enabled)
            exStyle |= (WS_EX_TRANSPARENT | WS_EX_LAYERED);
        else
        {
            exStyle &= ~WS_EX_TRANSPARENT;
            // NOTE: Window opacity also needs the layered window style so do not
            //       remove it if the window is alpha blended
            if (exStyle & WS_EX_LAYERED)
            {
                if (!(flags & LWA_ALPHA))
                    exStyle &= ~WS_EX_LAYERED;
            }
        }

        SetWindowLongW(s_WindowHandle, GWL_EXSTYLE, exStyle);

        if (enabled)
            SetLayeredWindowAttributes(s_WindowHandle, key, alpha, flags);

        data.mousePassthrough = enabled;
    }

    void WindowsWindow::SetFloating(bool enabled) {
        const HWND after = enabled ? HWND_TOPMOST : HWND_NOTOPMOST;
        SetWindowPos(s_WindowHandle, after, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

        data.floating = enabled;
    }

    void WindowsWindow::SetTransparentFramebuffer(bool enabled) {
        BOOL composition, opaque;
        DWORD color;

        if (!NEB_IsWindowsVistaOrGreater()) {
            return;
        }

        if (FAILED(DwmIsCompositionEnabled(&composition)) || !composition) {
            return;
        }

        if (NEB_IsWindows8OrGreater() ||
            (SUCCEEDED(DwmGetColorizationColor(&color, &opaque)) && !opaque))
        {
            HRGN region = CreateRectRgn(0, 0, -1, -1);
            DWM_BLURBEHIND bb = {0};
            bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
            bb.hRgnBlur = region;
            bb.fEnable = TRUE;

            DwmEnableBlurBehindWindow(s_WindowHandle, &bb);
            DeleteObject(region);
        }
        else
        {
            // HACK: Disable framebuffer transparency on Windows 7 when the
            //       colorization color is opaque, because otherwise the window
            //       contents is blended additively with the previous frame instead
            //       of replacing it
            DWM_BLURBEHIND bb = {0};
            bb.dwFlags = DWM_BB_ENABLE;
            DwmEnableBlurBehindWindow(s_WindowHandle, &bb);
        }

        data.transparent = enabled;
    }

    void WindowsWindow::SetDecorated(bool enabled) {
        if (enabled) {
            SetWindowLong(s_WindowHandle, GWL_STYLE, data.fullscreen ? s_DefaultFullscreenStyle : s_DefaultWindowedStyle);
        } else {
            SetWindowLong(s_WindowHandle, GWL_STYLE, WS_VISIBLE | WS_POPUP);
        }
        SetWindowSize(data.width, data.height);

        data.decorated = enabled;
    }

    std::chrono::steady_clock::time_point currentFrameTime;
    std::chrono::steady_clock::time_point lastFrameTime;
    void WindowsWindow::OnUpdate()
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

        MSG msg;
        while (PeekMessage(&msg, s_WindowHandle, NULL, NULL, PM_REMOVE) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        m_Context->SwapBuffers();

        currentFrameTime = std::chrono::high_resolution_clock::now();
        float diff = (float)std::chrono::duration_cast<std::chrono::milliseconds>(currentFrameTime - lastFrameTime).count();
        lastFrameTime = currentFrameTime;
        
        frameTime = diff / 1000.0f;
    }

    
    float WindowsWindow::GetTime() {
        return frameTime;
    }

    uint32 WindowsWindow::GetWidth() const
    {
        return data.width;
    }


    uint32 WindowsWindow::GetHeight() const
    {
        return data.height;
    }

    uint32* WindowsWindow::GetWidthPtr() const 
    {
        return &(data.width);
    }
    uint32* WindowsWindow::GetHeightPtr() const
    {
        return &(data.height);
    }

    
    void WindowsWindow::SetWindowSize(uint32 width, uint32 height)
    {
        LPRECT rect = (LPRECT)malloc(sizeof(RECT));
        if (!GetWindowRect(s_WindowHandle, rect)) {
            HandleError();
            return;
        }

        data.posX = rect->left;
        data.posY = rect->top;

        HWND windowTopOrTopmost = data.floating ? HWND_TOPMOST : HWND_TOP;
        bool err = !SetWindowPos(s_WindowHandle,  windowTopOrTopmost,
            data.posX, data.posY, width, height,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW);
            
        if (err) {
            HandleError();
            return;
        }

        data.width = width;
        data.height = height;
    }

    void WindowsWindow::UpdateWindowAttribs() {
        SetDecorated(data.decorated);
        SetPassthrough(data.mousePassthrough);
        SetTransparentFramebuffer(data.transparent);

        // Setting win size also sets floating info
        SetWindowSize(data.width, data.height);
    }  

    void WindowsWindow::ShutDown()
    {
        NEB_PROFILE_FUNCTION();
        DestroyWindow(s_WindowHandle);
    }

    void WindowsWindow::SetVSync(bool enabled)
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

    bool WindowsWindow::IsVSync() const
    {
        return data.vsync;
    }

    void WindowsWindow::SetIcon(std::string filepath)
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

        // Set win32 window icon using stbi image input

        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(bmi));
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = height;
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage = width * height * 4;

        HDC hdc = GetDC(s_WindowHandle);
        if(!hdc) {
            LOG_ERR("Failed to get dc for icon\n");
            HandleError();
            return;
        }

        HBITMAP hbm = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, NULL, NULL, 0);
        if(!hbm) {
            LOG_ERR("Failed to create bitmap for icon\n");
            HandleError();
            return;
        }
        HDC hdcMem = CreateCompatibleDC(hdc);
        if(!hdcMem) {
            LOG_ERR("Failed to create compatible dc for icon\n");
            HandleError();
            return;
        }

        HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, hbm);
        if(!hbmOld) {
            LOG_ERR("Failed to select object for icon\n");
            HandleError();
            return;
        }

        // Draw icon to hdc
        if(!StretchDIBits(hdcMem, 0, 0, width, height, 0, 0, width, height, data, &bmi, DIB_RGB_COLORS, SRCCOPY)) {
            LOG_ERR("Failed to draw icon to dc\n");
            HandleError();
            return;
        }

        // Using the BITMAPINFOHEADER to specify the icon bitmap.
        ICONINFO iconInfo;
        memset(&iconInfo, 0, sizeof(ICONINFO));
        iconInfo.fIcon = TRUE;
        iconInfo.hbmColor = hbm;
        iconInfo.hbmMask = hbm;
    
        // Set icon
        HICON hIcon = CreateIconIndirect(&iconInfo);
        if(!hIcon) {
            LOG_ERR("Failed to create icon\n");
            HandleError();
            return;
        }
        SetClassLongPtr(s_WindowHandle, GCLP_HICON, (LONG_PTR)hIcon);

        // Cleanup
        SelectObject(hdcMem, hbmOld);
        DeleteObject(hbm);
        DeleteDC(hdcMem);
        ReleaseDC(s_WindowHandle, hdc);

    }

    void WindowsWindow::MaximizeWindow()
    {
        bool err = !ShowWindow(s_WindowHandle, SW_MAXIMIZE);
        if (err) {
            HandleError();
            return;
        }
        data.maximized = true;
    }
    void WindowsWindow::RestoreWindow()
    {
        bool err = !ShowWindow(s_WindowHandle, SW_RESTORE);
        if (err) {
            HandleError();
            return;
        }
        data.maximized = false;
    }

    void WindowsWindow::MinimizeWindow()
    {
        bool err = !ShowWindow(s_WindowHandle, SW_MINIMIZE);
        if (err) {
            HandleError();
            return;
        }
        data.maximized = false;
        data.minimized = true;
    }

    bool WindowsInput::IsKeyPressedAsyncInt(KeyCode key) 
    {
        return GetAsyncKeyState(KeyCodeMapNebToWin[key]) &1;
    }
    bool WindowsInput::IsMouseButtonPressedAsyncInt(MouseCode key) 
    { 
        return GetAsyncKeyState(MouseCodeMapNebToWin[key]) &1;
    }

    Vec2f WindowsInput::GetMousePosInt() 
    { 
        LPPOINT point = (LPPOINT)malloc(sizeof(POINT));
        bool err = !GetCursorPos(point);
        if (err) {
            HandleError_Int();
            return Vec2f();
        }

        return Vec2f((float)point->x, (float)point->y);
    }

    void WindowsInput::SetMousePosInt(Vec2f pos)
    {
        bool err = !SetCursorPos(pos.X, pos.Y);
        if (err) {
            HandleError_Int();
            return;
        }
    }
#else
    WindowsData WindowsWindow::data = WindowsData();
    
    void WindowsWindow::ToggleFullscreen() {}
    WindowsWindow::WindowsWindow(WindowInfo inf) {}
    WindowsWindow::~WindowsWindow() {}
    void WindowsWindow::HandleError() {}
    void WindowsWindow::OnUpdate() {} 
    float WindowsWindow::GetTime() { return 0; }
    uint32 WindowsWindow::GetWidth() const { return 0; }
    uint32 WindowsWindow::GetHeight() const { return 0; }
    void WindowsWindow::SetWindowSize(uint32 width, uint32 height) {}
    void WindowsWindow::SetResizeable(bool resizeable) {};
    uint32* WindowsWindow::GetWidthPtr() const { return nullptr; }
    uint32* WindowsWindow::GetHeightPtr() const { return nullptr; }
    void WindowsWindow::SetIcon(std::string filepath) {}
    void WindowsWindow::MaximizeWindow() {}
    void WindowsWindow::RestoreWindow()  {} 
    void WindowsWindow::MinimizeWindow() {}
    Vec2u WindowsWindow::GetMaxWindowSize() { return Vec2u(0, 0); }
    void WindowsWindow::SetPassthrough(bool enabled) {}
    void WindowsWindow::SetFloating(bool enabled) {}
    void WindowsWindow::SetTransparentFramebuffer(bool enabled) {}
    void WindowsWindow::SetDecorated(bool enabled) {}
    void WindowsWindow::SetVSync(bool enabled) {}
    bool WindowsWindow::IsVSync() const { return true; }
    void* WindowsWindow::GetNativeWindow() { return nullptr; }
    void WindowsWindow::SwapIO(std::string in, std::string out, std::string err) {}
    void WindowsWindow::EnableConsole()  {}
    void WindowsWindow::DisableConsole() {}
    void WindowsWindow::ShutDown() {}
    bool WindowsWindow::SetWindowStyleVar(int style, bool enable) { return true; }
    void WindowsWindow::UpdateWindowAttribs() {}

    bool WindowsInput::IsKeyPressedAsyncInt(KeyCode key) { return true; }
    bool WindowsInput::IsMouseButtonPressedAsyncInt(MouseCode key) { return true;}
    Vec2f WindowsInput::GetMousePosInt() { return Vec2f(0.0f, 0.0f); }
    void WindowsInput:: SetMousePosInt(Vec2f newPos) {}

#endif
}