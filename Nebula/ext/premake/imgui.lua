src_dir = "../imgui"
build_dir = src_dir .. "/build"

target_dir = "../lib/"

project "imgui"
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
        src_dir .. "/*.c",
        src_dir .. "/*.h",
        src_dir .. "/*.cpp"
    }

    filter "system:macosx"
        buildoptions
        {
            "-x objective-c++"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
