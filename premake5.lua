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
        include "Nebula/ext/glfw"
        include "Nebula/ext/glad"
        include "Nebula/ext/imgui"
        include "Nebula/ext/freetype"
    group ""
    
project "Engine"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
    debugdir("./")
    files
    {
        "Nebula/src/**.h",
        "Nebula/src/**.cpp",
        "Nebula/ext/stb_image/**.cpp",
        "Nebula/ext/freetype-gl/**.c"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "FT2_BUILD_LIBRARY"
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
        "Nebula/ext/freetype/include"
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


-- project "NebulaStudio"
--     kind "StaticLib"
--     language "C++"
--     cppdialect "C++17"
--     staticruntime "on"  
    
--     targetdir ("bin/" .. outputdir .. "/%{prj.name}")
--     objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    

--     files
--     {
--         "NebulaEditor/src/**.h",
--         "NebulaEditor/src/**.cpp"
--     }

--     defines
--     {
--         "_CRT_SECURE_NO_WARNINGS",
--         "FT2_BUILD_LIBRARY"
--     }

--     sysincludedirs
--     {
--         "NebulaStudio/src",
--         "NebulaStudio/src/Nebula",
--         "NebulaStudio/include",
--         "Nebula/src",
--         "Nebula/src/Nebula",
--         "Nebula/include",
--         "Nebula/ext/imgui"
--     }

--     links
--     {
--         -- "Engine"
--     }

--     filter "system:windows"
--         systemversion "latest"
--         links
--         {
--             "user32", 
--             "gdi32", 
--             "opengl32", 
--             "shell32"
--         }

--     filter "system:macosx"
--         systemversion "latest"
--         links
--         {
--             "IOKit.framework", 
--             "OpenGL.framework",
--             "Cocoa.framework",
--             "CoreVideo.framework"
--         }


--     filter "system:linux"
--         systemversion "latest"
--         defines
--         {
--             "GLFW_SUPPLIED",
--             "_LIBS_SUPPLIED"
--         }
--         links
--         {
--             "X11",
--             "GL",
--             "GLU",
--             "dl"
--         }

--     filter "configurations:Debug"
--         defines "NEB_DEBUG"
--         runtime "Debug"
--         symbols "on"

--     filter "configurations:Release"
--         defines "NEB_RELEASE"
--         runtime "Release"
--         optimize "on"