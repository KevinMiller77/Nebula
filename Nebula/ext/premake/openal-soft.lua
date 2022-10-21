src_dir = "../openal-soft"
build_dir = src_dir .. "/build"

lib_dir = "build/lib"
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
    
    filter "not system:windows"
        buildcommands(
            "echo Running CMake on openal-soft && " ..
            "mkdir -p " .. build_dir .. " && " ..
            "cmake -S " .. src_dir .. " -B " .. build_dir .. " " .. cmake_opts .. " -DCMAKE_BUILD_TYPE=" .. cmake_level .. " .." .. " && " ..
            "make -j4 install -C " .. build_dir .. " && " ..
            "mkdir -p " .. out_dir .. cmake_level .. " && " ..
            "cp " .. lib_dir .. "/*.a " .. out_dir .. cmake_level .. "&&" ..
            "rm -rf build"
        )
