workspace "Nebula"
    startproject "NebulaEngine"
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
        -- include "Nebula/ext/freetype"
        group ""
        
project "NebulaEngine"
        kind "ConsoleApp"
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
        "Nebula/ext/stb_image/**.cpp",
        "NebulaStudio/src/**.h",
        "NebulaStudio/src/**.cpp"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_SUPPLIED",
        "_LIBS_SUPPLIED"
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
        "NebulaStudio/src",
        "NebulaStudio/src/Nebula",
        "NebulaStudio/include"
    }

    links
    {
        "X11",
        "glfw",
        "GLU",
        "GL",
        "dl",
        "pthread",
        "glad",
        "imgui"
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


-- project "NebulaStudio"
--     kind "ConsoleApp"
--     language "C++"
--     cppdialect "C++17"
--     staticruntime "on"  
    
    
--     targetname("NebulaStudio")
--     targetdir ("bin/" .. outputdir .. "/%{prj.name}")
--     objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

--     debugdir("./NebulaStudio")
    

--     files
--     {
--         "NebulaStudio/src/**.h",
--         "NebulaStudio/src/**.cpp"
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
--         "Nebula",
--         "Nebula/ext/imgui",
--         "Nebula/include",
--         "Nebula/src/Nebula"
--     }

--     links
--     {
        
--         "glfw",
--         "glad",
--         "imgui",
--         "NebulaEngine"
--     }

--     filter "system:windows"
--         systemversion "latest"
--         links
--         {
--             "user32", 
--             "gdi32", 
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
        -- defines
        -- {
        --     "GLFW_SUPPLIED",
        --     "_LIBS_SUPPLIED"
        -- }

--     filter "configurations:Debug"
--         defines "NEB_DEBUG"
--         runtime "Debug"
--         symbols "on"

--     filter "configurations:Release"
--         defines "NEB_RELEASE"
--         runtime "Release"
--         optimize "on"