project "glslang-default-resource-limits"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("../../build/%{prj.name}")
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "glslang/StandAlone/resource_limits_c.cpp",
        "glslang/StandAlone/ResourceLimits.cpp",
	}

    includedirs {
        "glslang",
        "glslang/StandAlone/"
    }

	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"