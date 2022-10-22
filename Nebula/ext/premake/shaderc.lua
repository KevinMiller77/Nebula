src_dir = "../shaderc"
build_dir = src_dir .. "/build"

lib_dir = "build/lib"
out_dir = "../lib/"

cmake_opts = " -GNinja "

project "shaderc"
kind "Makefile"

    cmake_level = "Release"
    filter "configurations:Debug"
        cmake_level = "Debug"    
    
    objdir(build_dir)
    targetdir("../lib/" .. cmake_level)
    location(build_dir)
    
    library_ext = ".a"
    filter "system:windows"
        library_ext = ".lib"

    buildcommands{
        "{ECHO} Running CMake on shaderc", 
        "{MKDIR} " .. build_dir,
        "python " .. src_dir .. "/utils/git-sync-deps",
        "cmake -GNinja -S " .. src_dir .. " -B " .. build_dir .. " " .. cmake_opts .. " -DCMAKE_BUILD_TYPE=" .. cmake_level .. " ..",
        "ninja shaderc_util -j4 -C " .. build_dir,
        "ninja shaderc -j4 -C " .. build_dir,
        "{MKDIR} " .. out_dir .. cmake_level,
        "{COPYFILE} " .. build_dir .. "/libshaderc_util/*" .. library_ext .. " " .. out_dir .. cmake_level,
        "{COPYFILE} " .. build_dir .. "/libshaderc/*" .. library_ext .. " " .. out_dir .. cmake_level,
        "{RMDIR} build"
    }
