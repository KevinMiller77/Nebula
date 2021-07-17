#include "WindowsWindow.h"
#include "WindowsInput.h"

#include <Windows.h>

#include <stb_image/stb_image.h>

#include <Core/PlatformInfo.h>

#include <Core/VFS.h>
#include <Nebula_pch.h>

#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

HWND s_WindowHandle;

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Nebula{
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
            case WM_KEYDOWN: {

                // Bool that decides if key repeat
                bool repeat = lParam & 0xFFFF;

                if (WindowsInput::KeyCodeMapWinToNeb.find(wParam) != WindowsInput::KeyCodeMapWinToNeb.end()) {
                    if (repeat) {
                        KeyHeldEvent event((int)WindowsInput::KeyCodeMapWinToNeb[wParam]);
                        winRef->data.EventCallback(event);
                    } else {
                        Input::SetKeyPressed(WindowsInput::KeyCodeMapWinToNeb[wParam]);
                        KeyTypedEvent event((int)WindowsInput::KeyCodeMapWinToNeb[wParam]);
                        winRef->data.EventCallback(event);
                    }
                } else if (WindowsInput::MouseCodeMapWinToNeb.find(wParam) != WindowsInput::MouseCodeMapWinToNeb.end()) {
                    MouseButtonPressedEvent event(WindowsInput::MouseCodeMapWinToNeb[wParam]);
                    winRef->data.EventCallback(event);
                    Input::SetMouseButtonPressed(WindowsInput::MouseCodeMapWinToNeb[wParam]);
                }
                break;
            } 
            case WM_KEYUP: {
                if (WindowsInput::KeyCodeMapWinToNeb.find(wParam) != WindowsInput::KeyCodeMapWinToNeb.end()) {
                    KeyReleasedEvent event((int)WindowsInput::MouseCodeMapWinToNeb[wParam]);
                    winRef->data.EventCallback(event);
                    Input::SetKeyReleased(WindowsInput::KeyCodeMapWinToNeb[wParam]);
                } else if (WindowsInput::MouseCodeMapWinToNeb.find(wParam) != WindowsInput::MouseCodeMapWinToNeb.end()) {
                    MouseButtonReleasedEvent event(WindowsInput::MouseCodeMapWinToNeb[wParam]);
                    winRef->data.EventCallback(event);
                    Input::SetMouseButtonReleased(WindowsInput::MouseCodeMapWinToNeb[wParam]);
                }
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
            case WM_SETFOCUS: {
                WindowFocusEvent event = WindowFocusEvent();
                winRef->data.EventCallback(event);
            }
            case WM_KILLFOCUS: {
                WindowLostFocusEvent event = WindowLostFocusEvent();
                winRef->data.EventCallback(event);
            }
            default: 
                return DefWindowProc(hWnd, msg, wParam, lParam); 
        }
        return 0;
    }

    void HandleError() {
        DWORD error = GetLastError();
        char *message = NULL;
        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL,
            error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPSTR)&message,
            0,
            NULL);

        LOG_ERR("[Window] %s\n", message);
    }

    WindowsWindow::~WindowsWindow()
    {
        ShutDown();
    }

    void* WindowsWindow::GetNativeWindow() {
        return s_WindowHandle;
    }

    void WindowsWindow::CallWindowHints(WindowInfo inf)
    {
        // // glfwWindowHint(GLFW_MOUSE_PASSTHROUGH, inf.MousePassthrough);
        // glfwWindowHint(GLFW_DECORATED, inf.MousePassthrough ? false : inf.Decorated);
        // glfwWindowHint(GLFW_FLOATING, inf.Floating);
        // glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, inf.Transparent);
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
        data.height = inf.Height; data.width = inf.Width;

        LOG_INF("Creating window\n");

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
            assert(1, "Could not register window class");
        }

        s_WindowHandle = ::CreateWindow(wc.lpszClassName, 
                                        (LPCWSTR)inf.Title, 
                                        WS_OVERLAPPEDWINDOW, 
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

        CallWindowHints(inf);

        m_Context = GraphicsContext::Create(s_WindowHandle);
        m_Context->Init();

        SetVSync(true);



        // Set GLFW callbacks
        // glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
        // {
        //     WindowsData& data = *(WindowsData*)glfwGetWindowUserPointer(window);
        //     data.width = width;
        //     data.height = height;

        //     WindowResizeEvent event(Vec2u(data.width, data.height));
        //     data.EventCallback(event);
        // });

    }

    void WindowsWindow::SetResizeable(bool resizeable) const
    {
        data.resizable = resizeable;
        if(data.resizable)
        {
            ::SetWindowLong(s_WindowHandle, GWL_STYLE, ::GetWindowLong(s_WindowHandle, GWL_STYLE) | WS_MAXIMIZEBOX);
        }
        else {
            ::SetWindowLong(s_WindowHandle, GWL_STYLE, ::GetWindowLong(s_WindowHandle, GWL_STYLE) &~ WS_MAXIMIZEBOX);
        }
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
                DWORD Style = WS_POPUP | WS_VISIBLE;
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
            DWORD Style = WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_CLIPCHILDREN;

            SetWindowLongPtr(s_WindowHandle, GWL_STYLE, Style);
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
        // glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, enabled);
    }

    uint64_t lastFrameTime = 0;
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
        if (PeekMessage(&msg, s_WindowHandle, NULL, NULL, PM_REMOVE) > 0){
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        m_Context->SwapBuffers();

        uint64_t curTime = GetTime();
        frameTime = ((float)(curTime - lastFrameTime)) / 1000.0f;
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
        
        bool err = !SetWindowPos(s_WindowHandle, HWND_TOPMOST,
            data.posX, data.posY, width, height,
            SWP_FRAMECHANGED | SWP_SHOWWINDOW);
            
        if (err) {
            HandleError();
            return;
        }

        data.width = width;
        data.height = height;

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

        data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return data.VSync;
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

        HICON icon = CreateIconFromResource(data, width * height * channels, TRUE, 0x00030000);
        if (!icon) {
            LOG_ERR("Failed to create icon!\n");
            return;
        }
        
        // Set the icon for the window.
        // This is important so that the icon appears in the taskbar.
        if (!SetClassLongPtr(s_WindowHandle, GCLP_HICON, (LONG_PTR)icon)) {
            LOG_ERR("Failed to set class long pointer!\n");
            return;
        }

        DestroyIcon(icon);
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
            HandleError();
            return Vec2f();
        }

        return Vec2f((float)point->x, (float)point->y);
    }

    void WindowsInput::SetMousePosInt(Vec2f pos)
    {
        bool err = !SetCursorPos(pos.X, pos.Y);
        if (err) {
            HandleError();
            return;
        }
    }
}