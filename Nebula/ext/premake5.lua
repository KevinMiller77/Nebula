workspace "shaderc-and-friends"
    architecture "x86_64"
    startproject "testlinker"
    configurations 
    { 
        "Debug",
        "Release"
    }

    flags
    {
        "MultiProcessorCompile"
    }

    outputdir = "%{cfg.buildcfg}/%{cfg.system}%{cfg.architecture}"

    group "Dependencies"
        include "GenericCodeGen"
        include "OGLCompiler"
        include "OSDependent"
        include "MachineIndependent"
        include "SPIRV"
        include "SPVRemapper"
        include "glslang"
        include "glslang-default-resource-limits"
        include "SPIRV-Tools"
        include "SPIRV-Tools-opt"
        include "shaderc_util"
        include "shaderc"
        group ""


project "testlinker"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
    staticruntime "On"

    location("build/%{prj.name}")
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "testlinker/testlinker.cpp"
	}

    links {
        "GenericCodeGen",
        "OGLCompiler",
        "OSDependent",
        "MachineIndependent",
        "SPIRV",
        "SPVRemapper",
        "glslang",
        "glslang-default-resource-limits",
        "SPIRV-Tools",
        "SPIRV-Tools-opt",
        "shaderc_util",
        "shaderc"
    }

	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
