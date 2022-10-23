project "shaderc"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("build/%{prj.name}")
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    dependson {
        "shaderc_util"
    }
    
    links {
        "shaderc_util"
    }

	files
	{
        "shaderc/libshaderc/include/shaderc/shaderc.h",
        "shaderc/libshaderc/include/shaderc/shaderc.hpp",
        "shaderc/libshaderc/src/shaderc.cc",
        "shaderc/libshaderc/src/shaderc_private.h",
	}


    includedirs {
        "shaderc/libshaderc/include",
        "shaderc/libshaderc_util/include",
        "shaderc/build/include",

        "glslang",
        "spirv-headers/include",

        "SPIRV-Tools/include",
    }

    defines {
        "ENABLE_HLSL"
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