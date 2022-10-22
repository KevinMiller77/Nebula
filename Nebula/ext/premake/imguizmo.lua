src_dir = "../imguizmo"
build_dir = src_dir .. "/build"

target_dir = "../lib/"

project "imguizmo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    cmake_level = "Release"
    filter "configurations:Debug"
        cmake_level = "Debug"   

    targetdir ("../lib/" .. cmake_level)
    objdir (build_dir)
    location(build_dir)

    files
    {
        src_dir .. "/*.cpp",
        src_dir .. "/*.h"
    }

    externalincludedirs {
        "../imgui"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"