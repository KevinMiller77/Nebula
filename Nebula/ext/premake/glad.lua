src_dir = "../glad"
build_dir = src_dir .. "/build"

target_dir = "../lib/"

project "glad"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    cmake_level = "Release"
    filter "configurations:Debug"
        cmake_level = "Debug"   

    targetdir ("../lib/" .. cmake_level)
    objdir (build_dir)
    location(build_dir)

    files
    {
        "../../include/glad/glad.h",
        "../../include/KHR/khrplatform.h",
        src_dir .. "/glad.c"
    }

    sysincludedirs
    {
        "../../include"
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"