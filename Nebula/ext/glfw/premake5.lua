project "glfw"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/vulkan.c",
        "src/init.c",
        "src/input.c", 
        "src/context.c",
        "src/monitor.c",
        "src/glfw_window.c",
        "src/egl_context.c"
    }
    
    sysincludedirs
    {
        "include/GLFW",
        "../../include"
    }
    
    filter "system:windows"
        systemversion "latest"

        defines
        {
            "_GLFW_WIN32"
        }

        files
        {
            "src/win32_init.c", 
            "src/win32_joystick.c", 
            "src/win32_monitor.c", 
            "src/win32_time.c", 
            "src/win32_thread.c", 
            "src/win32_window.c", 
            "src/wgl_context.c", 
            "src/osmesa_context.c"
        }

    filter "system:linux"
        defines
        {
            "_GLFW_X11"
        }

        files
        {
            "src/x11_init.c",
            "src/linux_joystick.c", 
            "src/x11_monitor.c", 
            "src/posix_time.c", 
            "src/posix_thread.c", 
            "src/x11_window.c", 
            "src/osmesa_context.c", 
            "src/xkb_unicode.c", 
            "src/glx_context.c"
        }

    filter "system:macosx"
        defines
        {
            "_GLFW_COCOA"
        }

        files
        {
            "src/cocoa_platform.h",
            "src/cocoa_joystick.h",
            "src/posix_thread.h",
            "src/nsgl_context.h",
            "src/egl_context.h",
            "src/osmesa_context.h",
            "src/cocoa_init.m",
            "src/cocoa_joystick.m",
            "src/cocoa_monitor.m",
            "src/cocoa_window.m",
            "src/cocoa_time.c",
            "src/posix_thread.c",
            "src/nsgl_context.m", 
            "src/egl_context.c", 
            "src/osmesa_context.c"
        }

    filter "configurations:Debug"
    runtime "Debug"
    symbols "on"
    
    filter "configurations:Release"
    runtime "Release"
    optimize "on"