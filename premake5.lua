workspace "Nebula"
    architecture "x86_64"
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
        include "Nebula/ext/glad"
        include "Nebula/ext/imgui"
        include "Nebula/ext/yaml-cpp"
        include "Nebula/ext/nfd"
        include "Nebula/ext/imguizmo"
        include "Nebula/ext/SPIRV-Cross"
        include "Nebula/ext/GenericCodeGen"
        include "Nebula/ext/OGLCompiler"
        include "Nebula/ext/OSDependent"
        include "Nebula/ext/MachineIndependent"
        include "Nebula/ext/SPIRV"
        include "Nebula/ext/SPVRemapper"
        include "Nebula/ext/glslang"
        include "Nebula/ext/glslang-default-resource-limits"
        include "Nebula/ext/SPIRV-Tools"
        include "Nebula/ext/SPIRV-Tools-opt"
        include "Nebula/ext/shaderc_util"
        include "Nebula/ext/shaderc"
        group ""
        
project "NebulaEngine"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"
        
        targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
        objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")
        location ("build")


        -- pchheader("Nebula/Nebula_pch.h")
        -- pchsource("Nebula/src/Nebula_pch.h")

    files
    {
        "Nebula/src/Nebula/**.h",
        "Nebula/src/Nebula/**.cpp",
        "Nebula/src/Platform/**.h",
        "Nebula/src/Platform/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "AL_LIBTYPE_STATIC"
    }

    externalincludedirs
    {
        "Nebula/",
        "Nebula/src",
        "Nebula/src/Nebula",
        "Nebula/include",
        "Nebula/ext",
        "Nebula/ext/stb_image/",
        "Nebula/ext/imgui",
        "Nebula/ext/imguizmo",
        "Nebula/ext/glad/include",
        "Nebula/ext/yaml-cpp/include",
        "Nebula/ext/nfd/src/include",
        "Nebula/ext/SPIRV-Cross/",
        "Nebula/ext/shaderc/libshaderc/include",
        "Nebula/ext/shaderc/libshaderc_util/include",
    }

    links
    {
        "imgui",
        "imguizmo",
        "glad",
        "yaml-cpp",
        "nfd",
        "SPIRV-Cross",
        "GenericCodeGen",
        "OGLCompiler",
        "OSDependent",
        "MachineIndependent",
        "SPIRV",
        "SPVRemapper",
        "glslang",
        "glslang-default-resource-limits",
        "SPIRV-Tools",
        "SPIRV-Tools-opt",
        "shaderc_util",
        "shaderc"
    }

    filter "system:windows"
        
        links
        {
            "opengl32",
            "dwmapi"
        }
        files 
        {
            "Nebula/ext/imgui/backends/imgui_impl_win32.cpp"
        }

    filter "system:macosx"
        links
        {
            "IOKit.framework", 
            "OpenGL.framework",
            "Cocoa.framework",
            "CoreVideo.framework"
        }


    filter "system:linux"
        
        
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
    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")
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

    externalincludedirs
    {
        "NebulaStudio/src",
        "NebulaStudio/src/Nebula",
        "NebulaStudio/include",
        "Nebula",

        "Nebula/ext",
        "Nebula/ext/imgui",
        "Nebula/ext/imguizmo",
        "Nebula/ext/stb_image/",
        "Nebula/ext/yaml-cpp/include",
        "Nebula/ext/nfd/src/include",

        "Nebula/include",
        "Nebula/src/Nebula"
    }

    links
    {
        "NebulaEngine"
    }

    filter "system:windows"
        
        ignoredefaultlibraries
        {
            "user32", 
            "gdi32", 
            "shell32",
            "msvcrtd"
        }

    filter "system:macosx"
        buildoptions {
            "-stdlib=libc++"
        }

        
        externalincludedirs
        {
            "NebulaStudio/src/Platform/MacOS",
        }
        
        links
        {
            "c++",
            "IOKit.framework", 
            "OpenGL.framework",
            "Metal.framework",
            "QuartzCore.framework",
            "Foundation.framework",
            "AppKit.framework",
            "Cocoa.framework",
            "CoreGraphics.framework",
            "MetalKit.framework", 
            "pthread",
            "imgui",
            "imguizmo",
            "glad",
            "yaml-cpp",
            "nfd",
            "SPIRV-Cross",
            "GenericCodeGen",
            "OGLCompiler",
            "OSDependent",
            "MachineIndependent",
            "SPIRV",
            "SPVRemapper",
            "glslang",
            "glslang-default-resource-limits",
            "SPIRV-Tools",
            "SPIRV-Tools-opt",
            "shaderc_util",
            "shaderc",
        }


    filter "system:linux"
        
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
            "imguizmo",
            "glad",
            "yaml-cpp",
            "nfd",
            "SPIRV-Cross",
            "GenericCodeGen",
            "OGLCompiler",
            "OSDependent",
            "MachineIndependent",
            "SPIRV",
            "SPVRemapper",
            "glslang",
            "glslang-default-resource-limits",
            "SPIRV-Tools",
            "SPIRV-Tools-opt",
            "shaderc_util",
            "shaderc",
        }

    filter "configurations:Debug"
        defines "NEB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NEB_RELEASE"
        runtime "Release"
        optimize "on"
