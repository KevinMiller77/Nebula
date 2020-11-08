workspace "Nebula"
    startproject "NebulaStudio"
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
        include "Nebula/ext/glfw"
        include "Nebula/ext/glad"
        include "Nebula/ext/imgui"
        include "Nebula/ext/yaml-cpp"
        include "Nebula/ext/nativefiledialog"
        -- include "Nebula/ext/freetype"
        group ""
        
project "NebulaEngine"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"
        
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        -- pchheader("Nebula/Nebula_pch.h")
        -- pchsource("Nebula/src/Nebula_pch.h")

    files
    {
        "Nebula/src/**.h",
        "Nebula/src/**.cpp",
        "Nebula/ext/stb_image/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    sysincludedirs
    {
        "Nebula/",
        "Nebula/src",
        "Nebula/src/Nebula",
        "Nebula/include",
        "Nebula/ext/imgui",
		"Nebula/ext/glfw/include",
        "Nebula/ext/glad/include",
        "Nebula/ext/yaml-cpp/include",
        "Nebula/ext/nativefiledialog/src/include"
    }

    links
    {
        "imgui",
        "glad",
        "glfw",
        "yaml-cpp",
        "nfd"
        -- "freetype"
    }

    filter "system:windows"
        systemversion "latest"
        links
        {
            "opengl32"
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


project "NebulaStudio"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"  
    
    
    targetname("NebulaStudio")
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    debugdir("./NebulaStudio")
    

    files
    {
        "NebulaStudio/src/**.h",
        "NebulaStudio/src/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "FT2_BUILD_LIBRARY"
    }

    sysincludedirs
    {
        "NebulaStudio/src",
        "NebulaStudio/src/Nebula",
        "NebulaStudio/include",
        "Nebula",

        "Nebula/ext/imgui",
        "Nebula/ext/yaml-cpp/include",
        "Nebula/include",
        "Nebula/src/Nebula"
    }

    links
    {
        "NebulaEngine"
    }

    filter "system:windows"
        systemversion "latest"
        ignoredefaultlibraries
        {
            "user32", 
            "gdi32", 
            "shell32",
            "msvcrtd"
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
            "dl",
            "pthread",
            "imgui",
            "glad",
            "glfw",
            "yaml-cpp",
            "nfd"
        }

    filter "configurations:Debug"
        defines "NEB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NEB_RELEASE"
        runtime "Release"
        optimize "on"
