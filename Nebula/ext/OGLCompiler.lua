project "OGLCompiler"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("../../build/%{prj.name}")
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "glslang/OGLCompilersDLL/InitializeDll.cpp",
        "glslang/OGLCompilersDLL/InitializeDll.h",
	}


    filter "system:windows"
        defines {
            'WIN32',
            '_WINDOWS',
            'NDEBUG',
            "_HAS_EXCEPTIONS=0",
            "ENABLE_HLSL",
            "GLSLANG_OSINCLUDE_WIN32",
            "ENABLE_OPT=0"
        }
	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"