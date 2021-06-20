project "SPIRV-Cross"
    location "SPIRV-Cross"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "spirv.h",
        "spirv.hpp",
        "spirv_cfg.cpp",
        "spirv_cfg.hpp",
        "spirv_common.hpp",
        "spirv_cpp.cpp",
        "spirv_cpp.hpp",
        "spirv_cross.cpp",
        "spirv_cross.hpp",
        "spirv_cross_c.cpp",
        "spirv_cross_c.h",
        "spirv_cross_containers.hpp",
        "spirv_cross_error_handling.hpp",
        "spirv_cross_parsed_ir.cpp",
        "spirv_cross_parsed_ir.hpp",
        "spirv_cross_util.cpp",
        "spirv_cross_util.hpp",
        "spirv_glsl.cpp",
        "spirv_glsl.hpp",
        "spirv_hlsl.cpp",
        "spirv_hlsl.hpp",
        "spirv_msl.cpp",
        "spirv_msl.hpp",
        "spirv_parser.cpp",
        "spirv_parser.hpp",
        "spirv_reflect.cpp",
        "spirv_reflect.hpp"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"