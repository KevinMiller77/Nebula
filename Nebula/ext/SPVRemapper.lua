project "SPVRemapper"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("build/%{prj.name}")
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "glslang/SPIRV/SPVRemapper.cpp",
        "glslang/SPIRV/doc.cpp",
        "glslang/SPIRV/SPVRemapper.h",
        "glslang/SPIRV/doc.h",
	}

    filter "system:windows"
        defines {
            'WIN32',
            '_WINDOWS'
        }
	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"