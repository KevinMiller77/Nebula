src_dir = "../yaml-cpp"
build_dir = src_dir .. "/build"

lib_dir = "build/lib"
out_dir = "../lib/"


project "yaml-cpp"
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
		src_dir .. "/src/**.h",
		src_dir .. "/src/**.cpp"
	}

	includedirs
	{
		src_dir .. "/include"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"