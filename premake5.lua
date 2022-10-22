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
        include "Nebula/ext/premake/glad"
        include "Nebula/ext/premake/glslang"
        include "Nebula/ext/premake/imgui"
        include "Nebula/ext/premake/imguizmo"
        include "Nebula/ext/premake/libogg"
        include "Nebula/ext/premake/nativefiledialog"
        include "Nebula/ext/premake/openal-soft"
        include "Nebula/ext/premake/shaderc"
        include "Nebula/ext/premake/shaderc_util"
        include "Nebula/ext/premake/SPIRV-Cross"
        include "Nebula/ext/premake/yaml-cpp"
        include "Nebula/ext/premake/Vorbis"
        group ""
        
project "NebulaEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    location ("build")

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

    externalincludedirs
    {
        "Nebula/",
        "Nebula/src",
        "Nebula/src/Nebula",

        "Nebula/include",
        
        "Nebula/ext",
        "Nebula/ext/imgui",
        "Nebula/ext/ImGuizmo",
		"Nebula/ext/libogg/include",
		"Nebula/ext/minimp3/include",
        "Nebula/ext/nativefiledialog/src/include",
        "Nebula/ext/openal-soft/alc",
        "Nebula/ext/openal-soft/include",
        "Nebula/ext/shaderc/libshaderc/include",
        "Nebula/ext/SPIRV-Cross/",
		"Nebula/ext/Vorbis/include",
        "Nebula/ext/yaml-cpp/include"
    }

    links {
        "Nebula/ext/lib/%{cfg.buildcfg}/*.lib"
    }

    -- links
    -- {
    --     -- Provided by shaderc
    --     "shaderc.lib",
    --     "shaderc_util.lib",

    --     -- Provided by glslang
    --     "GenericCodeGend",
    --     "glslangd",
    --     "MachineIndependentd",
    --     "OGLCompilerd",
    --     "OSDependentd",
    --     "SPIRVd",
    --     "SPIRV-Toolsd",
    --     "SPIRV-Tools-optd",
        
    --     -- Not provided by gslang. Why? I don't really know. We should tell them to just build this for our sake. Yeah, we'll do that.
    --     "SPIRV-Cross",

    --     "imgui",
    --     "imguizmo",
    --     "nfd",
    --     "glad",
    --     "OpenAL32",
	-- 	"Vorbis",
    --     "yaml-cppd"
    -- }

    filter "system:windows"
        systemversion "latest"
        links
        {
            "opengl32",
            "dwmapi"
        }
        files 
        {
            "Nebula/src/Platform/OS/Windows/**.h",
            "Nebula/src/Platform/OS/Windows/**.cpp"
        }


    filter "system:macosx"
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
    
    
    targetname("NebulaStudio")
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    location ("build")

    debugdir("./NebulaStudio")
    

    files
    {
        "test.cpp"
        -- "NebulaStudio/src/**.h",
        -- "NebulaStudio/src/**.cpp"
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

        "Nebula/",
        "Nebula/src",
        "Nebula/src/Nebula",

        "Nebula/include",
        
        "Nebula/ext",
        "Nebula/ext/imgui",
        "Nebula/ext/ImGuizmo",
		"Nebula/ext/libogg/include",
		"Nebula/ext/minimp3/include",
        "Nebula/ext/nativefiledialog/src/include",
        "Nebula/ext/openal-soft/alc",
        "Nebula/ext/openal-soft/include",
        "Nebula/ext/shaderc/libshaderc/include",
        "Nebula/ext/SPIRV-Cross/",
		"Nebula/ext/Vorbis/include",
        "Nebula/ext/yaml-cpp/include"
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
            "pthread"
        }

    filter "configurations:Debug"
        defines "NEB_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "NEB_RELEASE"
        runtime "Release"
        optimize "on"
