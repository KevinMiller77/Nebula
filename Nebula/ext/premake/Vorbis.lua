
src_dir = "../Vorbis"
build_dir = src_dir .. "/build"

lib_dir = "build/lib"
out_dir = "../lib/"

project "Vorbis"
    kind "StaticLib"
    language "C"

    cmake_level = "Release"
    filter "configurations:Debug"
        cmake_level = "Debug"    
    
    objdir(build_dir)
    targetdir("../lib/" .. cmake_level)
    location(build_dir)

    group "Dependencies"
        include "libogg"
        group "" 

    includedirs
    {
        src_dir .. "/include",
        src_dir .. "/lib",
        "../libogg/include"
    }

    files
    {
        src_dir .. "/lib/**.h",
        src_dir .. "/lib/**.c"
    }
    excludes
    {
        src_dir .."/lib/barkmel.c",
        src_dir .."/lib/misc.c",
        src_dir .."/lib/psytune.c",
        src_dir .."/lib/tone.c"
    }

    links
    {
        "libogg"
    }

    defines
    {
        "LIBOGG_EXPORTS"
    }
    

    filter "system:windows"
        defines
        {
            "WIN32",
            "NDEBUG"
        }
    
    filter "not system:windows"
        excludes
        {
            src_dir .. "/lib/windows.*"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"