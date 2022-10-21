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
    
    filter "not system:windows"
        buildcommands(
            "echo Running CMake on shaderc && " ..
            "mkdir -p " .. build_dir .. " && " ..
            "cmake -S " .. src_dir .. " -B " .. build_dir .. " " .. cmake_opts .. " -DCMAKE_BUILD_TYPE=" .. cmake_level .. " .." .. " && " ..
            "ninja libshaderc_util.a -j4 -C " .. build_dir .. " && " ..
            "ninja libshaderc.a -j4 -C " .. build_dir .. " && " ..
            "mkdir -p " .. out_dir .. cmake_level .. " && " ..
            "cp " .. build_dir .. "/libshaderc_util/*.a " .. out_dir .. cmake_level .. "&&" ..
            "cp " .. build_dir .. "/libshaderc/*.a " .. out_dir .. cmake_level .. "&&" ..
            "rm -rf build"
        )
