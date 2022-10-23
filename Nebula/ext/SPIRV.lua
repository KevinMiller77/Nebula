project "SPIRV"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("build/%{prj.name}")
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    dependson {
        "GenericCodeGen",
        "OGLCompiler",
        "OSDependent",
        "MachineIndependent"
    }

	files
	{
        "glslang/SPIRV/*.cpp",
        "glslang/SPIRV/*.h"
	}

	includedirs
	{
        "glslang",
        "glslang/SPIRV",
        "glslang/build/include"
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