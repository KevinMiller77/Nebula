src_dir = "../yaml-cpp"
build_dir = src_dir .. "/build"

lib_dir = "build/lib"
out_dir = "../lib/"

project "yaml-cpp"
	kind "Makefile"
	language "C++"
	cppdialect "C++17"

    cmake_level = "Release"
    filter "configurations:Debug"
        cmake_level = "Debug"    
    
    objdir(build_dir)
    targetdir("../lib/" .. cmake_level)
    location(build_dir)

    library_ext = ".a"
    filter "system:windows"
        library_ext = ".lib"

	buildcommands {
        "{ECHO} Running CMake on yaml-cpp",
        "{MKDIR} " .. build_dir,
        "{ECHO} Bootstrapping CMake build on yaml-cpp",
        "cmake -S " .. src_dir .. " -B " .. build_dir .. " " .. cmake_opts .. " -DCMAKE_BUILD_TYPE=" .. cmake_level .. " ..",
        "{ECHO} Building yaml-cpp",
        "cmake --build " .. build_dir .. " --config " .. cmake_level .. " -j 6",
        "{MKDIR} " .. out_dir .. cmake_level,
        "{COPYFILE} " .. build_dir .. "/*" .. library_ext .. " " .. out_dir .. cmake_level
    }