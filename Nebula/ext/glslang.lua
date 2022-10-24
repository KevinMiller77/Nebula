project "glslang"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("../../build/%{prj.name}")
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "glslang/glslang/CInterface/glslang_c_interface.cpp",
        "glslang/glslang/Public/ShaderLang.h",
        "glslang/glslang/Include/arrays.h",
        "glslang/glslang/Include/BaseTypes.h",
        "glslang/glslang/Include/Common.h",
        "glslang/glslang/Include/ConstantUnion.h",
        "glslang/glslang/Include/glslang_c_interface.h",
        "glslang/glslang/Include/glslang_c_shader_types.h",
        "glslang/glslang/Include/InfoSink.h",
        "glslang/glslang/Include/InitializeGlobals.h",
        "glslang/glslang/Include/intermediate.h",
        "glslang/glslang/Include/PoolAlloc.h",
        "glslang/glslang/Include/ResourceLimits.h",
        "glslang/glslang/Include/ShHandle.h",
        "glslang/glslang/Include/SpirvIntrinsics.h",
        "glslang/glslang/Include/Types.h",
	}

    includedirs {
        "glslang"
    }

	filter "system:windows"
        defines {
            "ENABLE_HLSL"
        }

	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"