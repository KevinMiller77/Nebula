src_dir = "../SPIRV-Cross"
build_dir = src_dir .. "/build"

lib_dir = "build/lib"
out_dir = "../lib/"

project "SPIRV-Cross"
    location "SPIRV-Cross"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"

    cmake_level = "Release"
    filter "configurations:Debug"
        cmake_level = "Debug"    
    
    objdir(build_dir)
    targetdir("../lib/" .. cmake_level)
    location(build_dir)

    files
    {
        src_dir .. "/*.h",
        src_dir .. "/*.hpp",
        src_dir .. "/*.cpp",
    }

    postbuildcommands {
        "{RMDIR} SPIRV-Cross"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"