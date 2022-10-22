src_dir = "../imgui"
build_dir = src_dir .. "/build"

target_dir = "../lib/"

project "imgui"
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
        src_dir .. "/*.c",
        src_dir .. "/*.h",
        src_dir .. "/*.cpp"
    }

    sysincludedirs {
        src_dir
    }

    filter "system:macosx"
        buildoptions
        {
            "-x objective-c++"
        }
    filter "system:windows"
        files {
            src_dir .. "/backends/imgui_impl_win32.cpp",
            src_dir .. "/backends/imgui_impl_opengl3.cpp",
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
