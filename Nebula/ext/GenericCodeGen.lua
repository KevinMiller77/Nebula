project "GenericCodeGen"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("../../build/%{prj.name}")
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "glslang/glslang/GenericCodeGen/CodeGen.cpp",
        "glslang/glslang/GenericCodeGen/Link.cpp",
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