src_dir = "../openal-soft"
build_dir = src_dir .. "/build"

out_dir = "../lib/"


project "openal-soft"
kind "Makefile"

    cmake_level = "Release"
    filter "configurations:Debug"
        cmake_level = "Debug"    

    cmake_opts = "-DCMAKE_BUILD_TYPE=" .. cmake_level .. " -DLIBTYPE=STATIC"
    
    objdir(build_dir)
    targetdir("../lib/" .. cmake_level)
    location(build_dir)
    
    intermidiate_dir = build_dir .. "/" .. cmake_level

    library_ext = ".a"
    filter "system:windows"
        library_ext = ".lib"

    buildcommands{
        "{ECHO} Running CMake on openal-soft",
        "{MKDIR} " .. build_dir,
        "{ECHO} Bootstrapping CMake build on openal-soft",
        "cmake -S " .. src_dir .. " -B " .. build_dir .. " " .. cmake_opts .. " -DCMAKE_BUILD_TYPE=" .. cmake_level .. " ..",
        "{ECHO} Building openal-soft",
        "cmake --build " .. build_dir .. " --config " .. cmake_level .. " -j 6",
        "{MKDIR} " .. out_dir .. cmake_level,
        "{COPYFILE} " .. intermidiate_dir .. "/*" .. library_ext .. " " .. out_dir .. cmake_level
    }

    rebuildcommands {
        "{ECHO} No need to rebuild if platform has not changed"
    }
