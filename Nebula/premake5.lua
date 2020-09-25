workspace "Nebula"
    startproject "Engine"
    configurations 
    { 
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}/%{cfg.system}%{cfg.architecture}"

    
    group "Dependencies"
        include "ext/glfw"
        include "ext/glad"
        include "ext/imgui"
        include "ext/freetype"
    group ""
    
project "Engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    

    files
    {
        "src/**.h",
        "src/**.cpp",
        "ext/stb_image/**.cpp",
        "ext/freetype-gl/**.c"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "FT2_BUILD_LIBRARY"
    }

    sysincludedirs
    {
        "src",
        "src/Nebula",
        "include",
		"ext/glfw/include",
        "ext/glad/include",
        "ext/imgui",
        "ext/freetype/include",
    }

    links
    {
        "glad",
        "glfw",
        "imgui",
        "freetype"
    }

    filter "system:windows"
        systemversion "latest"
        links
        {
            "user32", 
            "gdi32", 
            "opengl32", 
            "shell32"
        }

    filter "system:macosx"
        systemversion "latest"
        links
        {
            "IOKit.framework", 
            "OpenGL.framework",
            "Cocoa.framework",
            "CoreVideo.framework"
        }


    filter "system:linux"
        systemversion "latest"
        defines
        {
            "GLFW_SUPPLIED",
            "_LIBS_SUPPLIED"
        }
        links
        {
            "X11",
            "GL",
            "GLU",
            "dl"
        }

    filter "configurations:Debug"
        defines "NEB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NEB_RELEASE"
        runtime "Release"
        optimize "on"