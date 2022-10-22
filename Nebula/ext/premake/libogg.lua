proj_dir = "../libogg"
build_dir = proj_dir .. "/build"

target_dir = "../lib/"


project "libogg"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    cmake_level = "Release"
    filter "configurations:Debug"
        cmake_level = "Debug"   

    targetdir ("../lib/" .. cmake_level)
    objdir (build_dir)
    location(build_dir)

    

	includedirs
	{
		proj_dir .. "/include"
	}

	files
	{
		proj_dir .. "/include/**.h",
		proj_dir .. "/src/**.c"
	}

	defines
	{
		"LIBOGG_EXPORTS"
	}   
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"WIN32",
			"NDEBUG",
			"_WINDOWS",
			"_USRDLL"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"