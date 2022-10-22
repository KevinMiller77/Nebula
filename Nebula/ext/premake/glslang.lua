src_dir = "../glslang"
build_dir = src_dir .. "/build"

lib_dir = "build/lib"
out_dir = "../lib/"

cmake_opts = "-D CMAKE_INSTALL_PREFIX=\"build\""

project "glslang"
    kind "Makefile"

    
    cmake_level = "Release"
    filter "configurations:Debug"
        cmake_level = "Debug"    
    
    objdir(build_dir)
    targetdir("../lib/" .. cmake_level)
    location(build_dir)

    buildoutputs {
        out_dir .. "GenericCodeGen.lib",
        out_dir .. "glslang.lib",
        out_dir .. "MachineIndependent.lib",
        out_dir .. "OGLCompiler.lib",
        out_dir .. "OSDependent.lib",
        out_dir .. "SPIRV.lib",
        out_dir .. "SPIRV-Tools.lib",
        out_dir .. "SPIRV-Tools-opt.lib",
    }
    
    library_ext = ".a"
    filter "system:windows"
        library_ext = ".lib"

    buildcommands {
        "{ECHO} Running CMake on glslang ",
        "{ECHO} Running CMake on gl ",
        "{MKDIR} " .. build_dir,
        "{ECHO} Creating make target for glslang",
        "{CHDIR} " .. src_dir,
        "python update_glslang_sources.py",
        "{CHDIR} ../premake",
        "cmake -S " .. src_dir .. " -B " .. build_dir .. " " .. cmake_opts .. " ..", 
        "{ECHO} Building glslang",
        "cmake --build " .. build_dir .. " --config " .. cmake_level .. " --target install -j 6",
        "{MKDIR} -p " .. out_dir .. cmake_level,
        "{COPYFILE} " .. lib_dir .. "/*" .. library_ext .. " " .. out_dir .. cmake_level,
        "{RMDIR} build"
    }

    rebuildcommands {
        "{ECHO} No need to rebuild if platform has not changed"
    }
