proj_dir = "../shaderc"
build_dir = proj_dir .. "/build"

target_dir = "../lib/"


project "shaderc_util"
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
        proj_dir .. "/third_party/glslang",
        proj_dir .. "/third_party/spirv-tools/include",
        proj_dir .. "/third_party/spirv-headers/include",
        proj_dir .. "/third_party/glslang",
		proj_dir .. "/libshaderc/include",
		proj_dir .. "/libshaderc_util/include"
	}

	files
	{
        proj_dir .. "/libshaderc_util/src/args.cc",
        proj_dir .. "/libshaderc_util/src/compiler.cc",
        proj_dir .. "/libshaderc_util/src/file_finder.cc",
        proj_dir .. "/libshaderc_util/src/io_shaderc.cc",
        proj_dir .. "/libshaderc_util/src/message.cc",
        proj_dir .. "/libshaderc_util/src/resources.cc",
        proj_dir .. "/libshaderc_util/src/shader_stage.cc",
        proj_dir .. "/libshaderc_util/src/spirv_tools_wrapper.cc",
        proj_dir .. "/libshaderc_util/src/version_profile.cc"
	}

    filter "system:windows"
        defines
        {
            "ENABLE_HLSL"
        }   

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"