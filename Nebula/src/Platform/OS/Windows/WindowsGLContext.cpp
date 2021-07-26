#include <Platform/Graphics/GL/GLContext.h>

#include <glad/glad.h>
#include <Core/PlatformInfo.h>

// See https://www.opengl.org/registry/specs/ARB/wgl_create_context.txt for all values
#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

// See https://www.opengl.org/registry/specs/ARB/wgl_pixel_format.txt for all values
#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

namespace Nebula {

    typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext,
        const int *attribList);
    wglCreateContextAttribsARB_type *wglCreateContextAttribsARB = nullptr;

    typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList,
        const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
    wglChoosePixelFormatARB_type *wglChoosePixelFormatARB = nullptr;

    typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
    PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = nullptr;
    void* GetAnyGLFuncAddress(const char *name)
    {
        void *p = (void *)wglGetProcAddress(name);
        if(p == 0 ||
            (p == (void*)0x1) || (p == (void*)0x2) || (p == (void*)0x3) ||
            (p == (void*)-1) )
        {
            HMODULE module = LoadLibraryA("opengl32.dll");
            p = (void *)GetProcAddress(module, name);
        }

        return p;
    }
    HWND s_Window = nullptr;

    void loadWGLFunctions() {
        // Before we can load extensions, we need a dummy OpenGL context, created using a dummy window.
        // We use a dummy window because you can only set the pixel format for a window once. For the
        // real window, we want to use wglChoosePixelFormatARB (so we can potentially specify options
        // that aren't available in PIXELFORMATDESCRIPTOR), but we can't load and use that before we
        // have a context.
        WNDCLASSA window_class = {};
        window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        window_class.lpfnWndProc = DefWindowProcA;
        window_class.hInstance = GetModuleHandle(0);
        window_class.lpszClassName = "Dummy_WGL_djuasiodwa";

        if (!RegisterClassA(&window_class)) {
            assert(false, "Failed to register dummy OpenGL window.");
        }

        HWND dummy_window = CreateWindowExA(
            0,
            window_class.lpszClassName,
            "Dummy OpenGL Window",
            0,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            0,
            0,
            window_class.hInstance,
            0);

        if (!dummy_window) {
            assert(false, "Failed to create dummy OpenGL window.");
        }

        HDC dummy_dc = GetDC(dummy_window);

        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

        int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
        if (!pixel_format) {
            assert(false, "Failed to find a suitable pixel format.");
        }
        if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) {
            assert(false, "Failed to set the pixel format.");
        }

        HGLRC dummy_context = wglCreateContext(dummy_dc);
        if (!dummy_context) {
            assert(false, "Failed to create a dummy OpenGL rendering context.");
        }

        if (!wglMakeCurrent(dummy_dc, dummy_context)) {
            
            assert(false, "Failed to activate dummy OpenGL rendering context.");
        }

        // init glad
        if (!gladLoadGLLoader((GLADloadproc)GetAnyGLFuncAddress)) {
            assert(false, "Could not load glad!");
        }

        wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress("wglCreateContextAttribsARB");
        wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress("wglChoosePixelFormatARB");
        wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");
    
        wglMakeCurrent(dummy_dc, 0);
        wglDeleteContext(dummy_context);
        ReleaseDC(dummy_window, dummy_dc);
        DestroyWindow(dummy_window);
    }

    bool GLContext::InitContext_Int(void* window) {
        if (!IsWindow((HWND)window)) {
            assert(true, "Attempted to create GL context with invalid Win32 HWND");
        }

        s_Window = (HWND)window;

        loadWGLFunctions();

        int pixel_format_attribs[] = {
            WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
            WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
            WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
            WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
            WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
            WGL_COLOR_BITS_ARB,         32,
            WGL_DEPTH_BITS_ARB,         24,
            WGL_STENCIL_BITS_ARB,       8,
            0
        };

        int pixel_format;
        UINT num_formats;
        HDC real_dc = GetDC((HWND)window);
        wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
        if (!num_formats) {
            assert(true, "Failed to set the OpenGL 4.6 pixel format.");
        }

        PIXELFORMATDESCRIPTOR pfd;
        DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
        if (!SetPixelFormat(real_dc, pixel_format, &pfd)) {
            assert(true, "Failed to set the OpenGL 4.6 pixel format.");
        }

        // Specify that we want to create an OpenGL 4.6 core profile context
        int gl46_attribs[] = {
            WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
            WGL_CONTEXT_MINOR_VERSION_ARB, 6,
            WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
            0,
        };

        HGLRC gl46_context = wglCreateContextAttribsARB(real_dc, 0, gl46_attribs);
        if (!gl46_context) {
            assert(true, "Failed to create OpenGL 4.6 context.");
        }

        if (!wglMakeCurrent(real_dc, gl46_context)) {
            assert(true, "Failed to activate OpenGL 4.6 rendering context.");
        }

        s_DeviceContext = real_dc;
        return true;
    }

    bool GLContext::SwapBuffers_Int() {
        return ::SwapBuffers((HDC)s_DeviceContext);
    }

    void GLContext::SetVSync_Int(bool vsync) {
        assert(wglSwapIntervalEXT != NULL);

        if (vsync) {
            if (wglSwapIntervalEXT) {
                wglSwapIntervalEXT(1);
            }
        } else {   
            if (wglSwapIntervalEXT) {
                wglSwapIntervalEXT(0);
            }
        }
    }
}