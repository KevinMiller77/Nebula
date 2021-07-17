workspace "Nebula"
    architecture "x86_64"
    startproject "SpaceSim"
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
        include "Nebula/ext/glad"
        include "Nebula/ext/imgui"
        include "Nebula/ext/yaml-cpp"
        include "Nebula/ext/nativefiledialog"
        include "Nebula/ext/libogg"
        include "Nebula/ext/Vorbis"
        include "Nebula/ext/OpenAL-Soft"
        include "Nebula/ext/imguizmo"
        include "Nebula/ext/SPIRV-Cross"
        -- include "Nebula/ext/freetype"
        group ""
        
project "NebulaEngine"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"
        
        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
        location ("build")


        -- pchheader("Nebula/Nebula_pch.h")
        -- pchsource("Nebula/src/Nebula_pch.h")

    files
    {
        "Nebula/src/Nebula/**.h",
        "Nebula/src/Nebula/**.cpp",
        "Nebula/src/Platform/Graphics/**.h",
        "Nebula/src/Platform/Graphics/**.cpp",
        "Nebula/ext/stb_image/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "AL_LIBTYPE_STATIC"
    }

    sysincludedirs
    {
        "Nebula/",
        "Nebula/src",
        "Nebula/src/Nebula",
        "Nebula/include",
        "Nebula/ext",
        "Nebula/ext/imgui",
        "Nebula/ext/imguizmo",
        "Nebula/ext/glad/include",
        "Nebula/ext/yaml-cpp/include",
        "Nebula/ext/nativefiledialog/src/include",
        "Nebula/ext/OpenAL-Soft/include",
		"Nebula/ext/OpenAL-Soft/src",
		"Nebula/ext/OpenAL-Soft/src/common",
		"Nebula/ext/libogg/include",
		"Nebula/ext/Vorbis/include",
		"Nebula/ext/minimp3",
        "Nebula/ext/SPIRV-Cross/",
        "Nebula/ext/shaderc/include"
    }

    links
    {
        "imgui",
        "imguizmo",
        "glad",
        "yaml-cpp",
        "nfd",
        "OpenAL-Soft",
		"Vorbis",
        "SPIRV-Cross",
        -- "freetype"
    }

    filter "system:windows"
        systemversion "latest"
        links
        {
            "opengl32",
        }
        files 
        {
            "Nebula/src/Platform/OS/Windows/**.h",
            "Nebula/src/Platform/OS/Windows/**.cpp",
            "Nebula/ext/imgui/examples/imgui_impl_win32.cpp"
        }
        filter "configurations:Release"
        links {
                "Nebula/ext/shaderc/lib/Release/shaderc.lib",
                "Nebula/ext/shaderc/lib/Release/shaderc_util.lib",
                "Nebula/ext/glslang/lib/Release/GenericCodeGen.lib",
                "Nebula/ext/glslang/lib/Release/glslang.lib",
                "Nebula/ext/glslang/lib/Release/MachineIndependent.lib",
                "Nebula/ext/glslang/lib/Release/OGLCompiler.lib",
                "Nebula/ext/glslang/lib/Release/OSDependent.lib",
                "Nebula/ext/glslang/lib/Release/SPIRV.lib",
                "Nebula/ext/SPIRV-Tools/lib/Release/SPIRV-Tools.lib",
                "Nebula/ext/SPIRV-Tools/lib/Release/SPIRV-Tools-opt.lib",
            }
        filter "configurations:Debug"
            links 
            {
                "Nebula/ext/shaderc/lib/Debug/shaderc.lib",
                "Nebula/ext/shaderc/lib/Debug/shaderc_util.lib",
                "Nebula/ext/glslang/lib/Debug/GenericCodeGend.lib",
                "Nebula/ext/glslang/lib/Debug/glslangd.lib",
                "Nebula/ext/glslang/lib/Debug/MachineIndependentd.lib",
                "Nebula/ext/glslang/lib/Debug/OGLCompilerd.lib",
                "Nebula/ext/glslang/lib/Debug/OSDependentd.lib",
                "Nebula/ext/glslang/lib/Debug/SPIRVd.lib",
                "Nebula/ext/SPIRV-Tools/lib/Debug/SPIRV-Tools.lib",
                "Nebula/ext/SPIRV-Tools/lib/Debug/SPIRV-Tools-opt.lib",
            }


    filter "system:macosx"
        systemversion "latest"
        
        files 
        {
            "Nebula/src/Platform/OS/MacOS/**.h",
            "Nebula/src/Platform/OS/MacOS/**.cpp",
        }
        links
        {
            "IOKit.framework", 
            "OpenGL.framework",
            "Cocoa.framework",
            "CoreVideo.framework"
        }


    filter "system:linux"
        systemversion "latest"
        
        files 
        {
            "Nebula/src/Platform/OS/Linux/**.h",
            "Nebula/src/Platform/OS/Linux/**.cpp",
        }
        defines
        {
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
    location ("build")

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
        "Nebula/ext/imguizmo",
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
