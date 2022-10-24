project "spirv-tools"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("../../build/%{prj.name}")
    targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
        "SPIRV-Tools/include/spirv-tools/libspirv.h",
        "SPIRV-Tools/source/util/bitutils.h",
        "SPIRV-Tools/source/util/bit_vector.h",
        "SPIRV-Tools/source/util/hash_combine.h",
        "SPIRV-Tools/source/util/hex_float.h",
        "SPIRV-Tools/source/util/make_unique.h",
        "SPIRV-Tools/source/util/parse_number.h",
        "SPIRV-Tools/source/util/small_vector.h",
        "SPIRV-Tools/source/util/string_utils.h",
        "SPIRV-Tools/source/util/timer.h",
        "SPIRV-Tools/source/assembly_grammar.h",
        "SPIRV-Tools/source/binary.h",
        "SPIRV-Tools/source/cfa.h",
        "SPIRV-Tools/source/common_debug_info.h",
        "SPIRV-Tools/source/diagnostic.h",
        "SPIRV-Tools/source/disassemble.h",
        "SPIRV-Tools/source/enum_set.h",
        "SPIRV-Tools/source/enum_string_mapping.h",
        "SPIRV-Tools/source/ext_inst.h",
        "SPIRV-Tools/source/extensions.h",
        "SPIRV-Tools/source/instruction.h",
        "SPIRV-Tools/source/latest_version_glsl_std_450_header.h",
        "SPIRV-Tools/source/latest_version_opencl_std_header.h",
        "SPIRV-Tools/source/latest_version_spirv_header.h",
        "SPIRV-Tools/source/macro.h",
        "SPIRV-Tools/source/name_mapper.h",
        "SPIRV-Tools/source/opcode.h",
        "SPIRV-Tools/source/operand.h",
        "SPIRV-Tools/source/parsed_operand.h",
        "SPIRV-Tools/source/print.h",
        "SPIRV-Tools/source/spirv_constant.h",
        "SPIRV-Tools/source/spirv_definition.h",
        "SPIRV-Tools/source/spirv_endian.h",
        "SPIRV-Tools/source/spirv_fuzzer_options.h",
        "SPIRV-Tools/source/spirv_optimizer_options.h",
        "SPIRV-Tools/source/spirv_reducer_options.h",
        "SPIRV-Tools/source/spirv_target_env.h",
        "SPIRV-Tools/source/spirv_validator_options.h",
        "SPIRV-Tools/source/table.h",
        "SPIRV-Tools/source/text.h",
        "SPIRV-Tools/source/text_handler.h",
        "SPIRV-Tools/source/val/validate.h",

        "SPIRV-Tools/source/util/bit_vector.cpp",
        "SPIRV-Tools/source/util/parse_number.cpp",
        "SPIRV-Tools/source/util/string_utils.cpp",
        "SPIRV-Tools/source/assembly_grammar.cpp",
        "SPIRV-Tools/source/binary.cpp",
        "SPIRV-Tools/source/diagnostic.cpp",
        "SPIRV-Tools/source/disassemble.cpp",
        "SPIRV-Tools/source/enum_string_mapping.cpp",
        "SPIRV-Tools/source/ext_inst.cpp",
        "SPIRV-Tools/source/extensions.cpp",
        "SPIRV-Tools/source/libspirv.cpp",
        "SPIRV-Tools/source/name_mapper.cpp",
        "SPIRV-Tools/source/opcode.cpp",
        "SPIRV-Tools/source/operand.cpp",
        "SPIRV-Tools/source/parsed_operand.cpp",
        "SPIRV-Tools/source/print.cpp",
        "SPIRV-Tools/source/software_version.cpp",
        "SPIRV-Tools/source/spirv_endian.cpp",
        "SPIRV-Tools/source/spirv_fuzzer_options.cpp",
        "SPIRV-Tools/source/spirv_optimizer_options.cpp",
        "SPIRV-Tools/source/spirv_reducer_options.cpp",
        "SPIRV-Tools/source/spirv_target_env.cpp",
        "SPIRV-Tools/source/spirv_validator_options.cpp",
        "SPIRV-Tools/source/table.cpp",
        "SPIRV-Tools/source/text.cpp",
        "SPIRV-Tools/source/text_handler.cpp",
        "SPIRV-Tools/source/val/validate.cpp",
        "SPIRV-Tools/source/val/validate_adjacency.cpp",
        "SPIRV-Tools/source/val/validate_annotation.cpp",
        "SPIRV-Tools/source/val/validate_arithmetics.cpp",
        "SPIRV-Tools/source/val/validate_atomics.cpp",
        "SPIRV-Tools/source/val/validate_barriers.cpp",
        "SPIRV-Tools/source/val/validate_bitwise.cpp",
        "SPIRV-Tools/source/val/validate_builtins.cpp",
        "SPIRV-Tools/source/val/validate_capability.cpp",
        "SPIRV-Tools/source/val/validate_cfg.cpp",
        "SPIRV-Tools/source/val/validate_composites.cpp",
        "SPIRV-Tools/source/val/validate_constants.cpp",
        "SPIRV-Tools/source/val/validate_conversion.cpp",
        "SPIRV-Tools/source/val/validate_debug.cpp",
        "SPIRV-Tools/source/val/validate_decorations.cpp",
        "SPIRV-Tools/source/val/validate_derivatives.cpp",
        "SPIRV-Tools/source/val/validate_extensions.cpp",
        "SPIRV-Tools/source/val/validate_execution_limitations.cpp",
        "SPIRV-Tools/source/val/validate_function.cpp",
        "SPIRV-Tools/source/val/validate_id.cpp",
        "SPIRV-Tools/source/val/validate_image.cpp",
        "SPIRV-Tools/source/val/validate_interfaces.cpp",
        "SPIRV-Tools/source/val/validate_instruction.cpp",
        "SPIRV-Tools/source/val/validate_layout.cpp",
        "SPIRV-Tools/source/val/validate_literals.cpp",
        "SPIRV-Tools/source/val/validate_logicals.cpp",
        "SPIRV-Tools/source/val/validate_memory.cpp",
        "SPIRV-Tools/source/val/validate_memory_semantics.cpp",
        "SPIRV-Tools/source/val/validate_mesh_shading.cpp",
        "SPIRV-Tools/source/val/validate_misc.cpp",
        "SPIRV-Tools/source/val/validate_mode_setting.cpp",
        "SPIRV-Tools/source/val/validate_non_uniform.cpp",
        "SPIRV-Tools/source/val/validate_primitives.cpp",
        "SPIRV-Tools/source/val/validate_ray_query.cpp",
        "SPIRV-Tools/source/val/validate_ray_tracing.cpp",
        "SPIRV-Tools/source/val/validate_scopes.cpp",
        "SPIRV-Tools/source/val/validate_small_type_uses.cpp",
        "SPIRV-Tools/source/val/validate_type.cpp",
        "SPIRV-Tools/source/val/basic_block.cpp",
        "SPIRV-Tools/source/val/construct.cpp",
        "SPIRV-Tools/source/val/function.cpp",
        "SPIRV-Tools/source/val/instruction.cpp",
        "SPIRV-Tools/source/val/validation_state.cpp",
        "SPIRV-Tools/source/pch_source.cpp"
	}

	includedirs
	{
        "SPIRV-Tools",
		"SPIRV-Tools/include",
		"SPIRV-Tools/build",
		"SPIRV-Headers/include",
	}
    

    p = path.getabsolute(".")

    filter "system:windows"
        prebuildcommands { 
            "xcopy /Q /E /Y /I \"" .. p .. "/SPIRV-Headers\" \"" .. p .. "/SPIRV-Tools/external/SPIRV-Headers\"",
            "{MKDIR} ".. p .. "/SPIRV-Tools/build",
            "cmake -D SKIP_SPIRV_TOOLS_INSTALL=ON -S " .. p .. "/SPIRV-Tools -B " .. p .. "/SPIRV-Tools/build",
            "cmake --build " .. p .. "/SPIRV-Tools/build --target SPIRV-Tools-static -j6"
        }

        defines {
            'WIN32',
            '_WINDOWS',
            'NDEBUG',
            '_CRT_SECURE_NO_WARNINGS',
            '_SCL_SECURE_NO_WARNINGS',
            'SPIRV_WINDOWS;SPIRV_COLOR_TERMINAL',
            'CMAKE_INTDIR="MinSizeRel"'
        }

    filter "not system:windows"
        prebuildcommands { 
            "{RMDIR} " .. p .. "/SPIRV-Tools/external/SPIRV-Headers",
            "ln -s -f " .. p .. "/SPIRV-Headers " .. p .. "/SPIRV-Tools/external/SPIRV-Headers",
            "{MKDIR} " .. p .. "/SPIRV-Tools/build",
            "cmake -D SKIP_SPIRV_TOOLS_INSTALL=ON -S " .. p .. "/SPIRV-Tools -B " .. p .. "/SPIRV-Tools/build",
            "cmake --build " .. p .. "/SPIRV-Tools/build --target SPIRV-Tools-static -j6"
        }
        
	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"