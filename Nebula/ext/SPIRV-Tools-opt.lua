project "spirv-tools-opt"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("../../build/%{prj.name}")
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    dependson("spirv-tools")

	files
	{
        "SPIRV-Tools/source/opt/fix_func_call_arguments.h",
        "SPIRV-Tools/source/opt/aggressive_dead_code_elim_pass.h",
        "SPIRV-Tools/source/opt/amd_ext_to_khr.h",
        "SPIRV-Tools/source/opt/basic_block.h",
        "SPIRV-Tools/source/opt/block_merge_pass.h",
        "SPIRV-Tools/source/opt/block_merge_util.h",
        "SPIRV-Tools/source/opt/build_module.h",
        "SPIRV-Tools/source/opt/ccp_pass.h",
        "SPIRV-Tools/source/opt/cfg_cleanup_pass.h",
        "SPIRV-Tools/source/opt/cfg.h",
        "SPIRV-Tools/source/opt/code_sink.h",
        "SPIRV-Tools/source/opt/combine_access_chains.h",
        "SPIRV-Tools/source/opt/compact_ids_pass.h",
        "SPIRV-Tools/source/opt/composite.h",
        "SPIRV-Tools/source/opt/const_folding_rules.h",
        "SPIRV-Tools/source/opt/constants.h",
        "SPIRV-Tools/source/opt/control_dependence.h",
        "SPIRV-Tools/source/opt/convert_to_sampled_image_pass.h",
        "SPIRV-Tools/source/opt/convert_to_half_pass.h",
        "SPIRV-Tools/source/opt/copy_prop_arrays.h",
        "SPIRV-Tools/source/opt/dataflow.h",
        "SPIRV-Tools/source/opt/dead_branch_elim_pass.h",
        "SPIRV-Tools/source/opt/dead_insert_elim_pass.h",
        "SPIRV-Tools/source/opt/dead_variable_elimination.h",
        "SPIRV-Tools/source/opt/decoration_manager.h",
        "SPIRV-Tools/source/opt/debug_info_manager.h",
        "SPIRV-Tools/source/opt/def_use_manager.h",
        "SPIRV-Tools/source/opt/desc_sroa.h",
        "SPIRV-Tools/source/opt/desc_sroa_util.h",
        "SPIRV-Tools/source/opt/dominator_analysis.h",
        "SPIRV-Tools/source/opt/dominator_tree.h",
        "SPIRV-Tools/source/opt/eliminate_dead_constant_pass.h",
        "SPIRV-Tools/source/opt/eliminate_dead_functions_pass.h",
        "SPIRV-Tools/source/opt/eliminate_dead_functions_util.h",
        "SPIRV-Tools/source/opt/eliminate_dead_input_components_pass.h",
        "SPIRV-Tools/source/opt/eliminate_dead_members_pass.h",
        "SPIRV-Tools/source/opt/empty_pass.h",
        "SPIRV-Tools/source/opt/feature_manager.h",
        "SPIRV-Tools/source/opt/fix_storage_class.h",
        "SPIRV-Tools/source/opt/flatten_decoration_pass.h",
        "SPIRV-Tools/source/opt/fold.h",
        "SPIRV-Tools/source/opt/folding_rules.h",
        "SPIRV-Tools/source/opt/fold_spec_constant_op_and_composite_pass.h",
        "SPIRV-Tools/source/opt/freeze_spec_constant_value_pass.h",
        "SPIRV-Tools/source/opt/function.h",
        "SPIRV-Tools/source/opt/graphics_robust_access_pass.h",
        "SPIRV-Tools/source/opt/if_conversion.h",
        "SPIRV-Tools/source/opt/inline_exhaustive_pass.h",
        "SPIRV-Tools/source/opt/inline_opaque_pass.h",
        "SPIRV-Tools/source/opt/inline_pass.h",
        "SPIRV-Tools/source/opt/inst_bindless_check_pass.h",
        "SPIRV-Tools/source/opt/inst_buff_addr_check_pass.h",
        "SPIRV-Tools/source/opt/inst_debug_printf_pass.h",
        "SPIRV-Tools/source/opt/instruction.h",
        "SPIRV-Tools/source/opt/instruction_list.h",
        "SPIRV-Tools/source/opt/instrument_pass.h",
        "SPIRV-Tools/source/opt/interface_var_sroa.h",
        "SPIRV-Tools/source/opt/interp_fixup_pass.h",
        "SPIRV-Tools/source/opt/ir_builder.h",
        "SPIRV-Tools/source/opt/ir_context.h",
        "SPIRV-Tools/source/opt/ir_loader.h",
        "SPIRV-Tools/source/opt/licm_pass.h",
        "SPIRV-Tools/source/opt/local_access_chain_convert_pass.h",
        "SPIRV-Tools/source/opt/local_redundancy_elimination.h",
        "SPIRV-Tools/source/opt/local_single_block_elim_pass.h",
        "SPIRV-Tools/source/opt/local_single_store_elim_pass.h",
        "SPIRV-Tools/source/opt/log.h",
        "SPIRV-Tools/source/opt/loop_dependence.h",
        "SPIRV-Tools/source/opt/loop_descriptor.h",
        "SPIRV-Tools/source/opt/loop_fission.h",
        "SPIRV-Tools/source/opt/loop_fusion.h",
        "SPIRV-Tools/source/opt/loop_fusion_pass.h",
        "SPIRV-Tools/source/opt/loop_peeling.h",
        "SPIRV-Tools/source/opt/loop_unroller.h",
        "SPIRV-Tools/source/opt/loop_utils.h",
        "SPIRV-Tools/source/opt/loop_unswitch_pass.h",
        "SPIRV-Tools/source/opt/mem_pass.h",
        "SPIRV-Tools/source/opt/merge_return_pass.h",
        "SPIRV-Tools/source/opt/module.h",
        "SPIRV-Tools/source/opt/null_pass.h",
        "SPIRV-Tools/source/opt/passes.h",
        "SPIRV-Tools/source/opt/pass.h",
        "SPIRV-Tools/source/opt/pass_manager.h",
        "SPIRV-Tools/source/opt/private_to_local_pass.h",
        "SPIRV-Tools/source/opt/propagator.h",
        "SPIRV-Tools/source/opt/reduce_load_size.h",
        "SPIRV-Tools/source/opt/redundancy_elimination.h",
        "SPIRV-Tools/source/opt/reflect.h",
        "SPIRV-Tools/source/opt/register_pressure.h",
        "SPIRV-Tools/source/opt/relax_float_ops_pass.h",
        "SPIRV-Tools/source/opt/remove_dontinline_pass.h",
        "SPIRV-Tools/source/opt/remove_duplicates_pass.h",
        "SPIRV-Tools/source/opt/remove_unused_interface_variables_pass.h",
        "SPIRV-Tools/source/opt/replace_desc_array_access_using_var_index.h",
        "SPIRV-Tools/source/opt/replace_invalid_opc.h",
        "SPIRV-Tools/source/opt/scalar_analysis.h",
        "SPIRV-Tools/source/opt/scalar_analysis_nodes.h",
        "SPIRV-Tools/source/opt/scalar_replacement_pass.h",
        "SPIRV-Tools/source/opt/set_spec_constant_default_value_pass.h",
        "SPIRV-Tools/source/opt/simplification_pass.h",
        "SPIRV-Tools/source/opt/spread_volatile_semantics.h",
        "SPIRV-Tools/source/opt/ssa_rewrite_pass.h",
        "SPIRV-Tools/source/opt/strength_reduction_pass.h",
        "SPIRV-Tools/source/opt/strip_debug_info_pass.h",
        "SPIRV-Tools/source/opt/strip_nonsemantic_info_pass.h",
        "SPIRV-Tools/source/opt/struct_cfg_analysis.h",
        "SPIRV-Tools/source/opt/tree_iterator.h",
        "SPIRV-Tools/source/opt/type_manager.h",
        "SPIRV-Tools/source/opt/types.h",
        "SPIRV-Tools/source/opt/unify_const_pass.h",
        "SPIRV-Tools/source/opt/upgrade_memory_model.h",
        "SPIRV-Tools/source/opt/value_number_table.h",
        "SPIRV-Tools/source/opt/vector_dce.h",
        "SPIRV-Tools/source/opt/workaround1209.h",
        "SPIRV-Tools/source/opt/wrap_opkill.h",
        
        "SPIRV-Tools/source/opt/fix_func_call_arguments.cpp",
        "SPIRV-Tools/source/opt/aggressive_dead_code_elim_pass.cpp",
        "SPIRV-Tools/source/opt/amd_ext_to_khr.cpp",
        "SPIRV-Tools/source/opt/basic_block.cpp",
        "SPIRV-Tools/source/opt/block_merge_pass.cpp",
        "SPIRV-Tools/source/opt/block_merge_util.cpp",
        "SPIRV-Tools/source/opt/build_module.cpp",
        "SPIRV-Tools/source/opt/ccp_pass.cpp",
        "SPIRV-Tools/source/opt/cfg_cleanup_pass.cpp",
        "SPIRV-Tools/source/opt/cfg.cpp",
        "SPIRV-Tools/source/opt/code_sink.cpp",
        "SPIRV-Tools/source/opt/combine_access_chains.cpp",
        "SPIRV-Tools/source/opt/compact_ids_pass.cpp",
        "SPIRV-Tools/source/opt/composite.cpp",
        "SPIRV-Tools/source/opt/const_folding_rules.cpp",
        "SPIRV-Tools/source/opt/constants.cpp",
        "SPIRV-Tools/source/opt/control_dependence.cpp",
        "SPIRV-Tools/source/opt/convert_to_sampled_image_pass.cpp",
        "SPIRV-Tools/source/opt/convert_to_half_pass.cpp",
        "SPIRV-Tools/source/opt/copy_prop_arrays.cpp",
        "SPIRV-Tools/source/opt/dataflow.cpp",
        "SPIRV-Tools/source/opt/dead_branch_elim_pass.cpp",
        "SPIRV-Tools/source/opt/dead_insert_elim_pass.cpp",
        "SPIRV-Tools/source/opt/dead_variable_elimination.cpp",
        "SPIRV-Tools/source/opt/decoration_manager.cpp",
        "SPIRV-Tools/source/opt/debug_info_manager.cpp",
        "SPIRV-Tools/source/opt/def_use_manager.cpp",
        "SPIRV-Tools/source/opt/desc_sroa.cpp",
        "SPIRV-Tools/source/opt/desc_sroa_util.cpp",
        "SPIRV-Tools/source/opt/dominator_analysis.cpp",
        "SPIRV-Tools/source/opt/dominator_tree.cpp",
        "SPIRV-Tools/source/opt/eliminate_dead_constant_pass.cpp",
        "SPIRV-Tools/source/opt/eliminate_dead_functions_pass.cpp",
        "SPIRV-Tools/source/opt/eliminate_dead_functions_util.cpp",
        "SPIRV-Tools/source/opt/eliminate_dead_input_components_pass.cpp",
        "SPIRV-Tools/source/opt/eliminate_dead_members_pass.cpp",
        "SPIRV-Tools/source/opt/feature_manager.cpp",
        "SPIRV-Tools/source/opt/fix_storage_class.cpp",
        "SPIRV-Tools/source/opt/flatten_decoration_pass.cpp",
        "SPIRV-Tools/source/opt/fold.cpp",
        "SPIRV-Tools/source/opt/folding_rules.cpp",
        "SPIRV-Tools/source/opt/fold_spec_constant_op_and_composite_pass.cpp",
        "SPIRV-Tools/source/opt/freeze_spec_constant_value_pass.cpp",
        "SPIRV-Tools/source/opt/function.cpp",
        "SPIRV-Tools/source/opt/graphics_robust_access_pass.cpp",
        "SPIRV-Tools/source/opt/if_conversion.cpp",
        "SPIRV-Tools/source/opt/inline_exhaustive_pass.cpp",
        "SPIRV-Tools/source/opt/inline_opaque_pass.cpp",
        "SPIRV-Tools/source/opt/inline_pass.cpp",
        "SPIRV-Tools/source/opt/inst_bindless_check_pass.cpp",
        "SPIRV-Tools/source/opt/inst_buff_addr_check_pass.cpp",
        "SPIRV-Tools/source/opt/inst_debug_printf_pass.cpp",
        "SPIRV-Tools/source/opt/instruction.cpp",
        "SPIRV-Tools/source/opt/instruction_list.cpp",
        "SPIRV-Tools/source/opt/instrument_pass.cpp",
        "SPIRV-Tools/source/opt/interface_var_sroa.cpp",
        "SPIRV-Tools/source/opt/interp_fixup_pass.cpp",
        "SPIRV-Tools/source/opt/ir_context.cpp",
        "SPIRV-Tools/source/opt/ir_loader.cpp",
        "SPIRV-Tools/source/opt/licm_pass.cpp",
        "SPIRV-Tools/source/opt/local_access_chain_convert_pass.cpp",
        "SPIRV-Tools/source/opt/local_redundancy_elimination.cpp",
        "SPIRV-Tools/source/opt/local_single_block_elim_pass.cpp",
        "SPIRV-Tools/source/opt/local_single_store_elim_pass.cpp",
        "SPIRV-Tools/source/opt/loop_dependence.cpp",
        "SPIRV-Tools/source/opt/loop_dependence_helpers.cpp",
        "SPIRV-Tools/source/opt/loop_descriptor.cpp",
        "SPIRV-Tools/source/opt/loop_fission.cpp",
        "SPIRV-Tools/source/opt/loop_fusion.cpp",
        "SPIRV-Tools/source/opt/loop_fusion_pass.cpp",
        "SPIRV-Tools/source/opt/loop_peeling.cpp",
        "SPIRV-Tools/source/opt/loop_utils.cpp",
        "SPIRV-Tools/source/opt/loop_unroller.cpp",
        "SPIRV-Tools/source/opt/loop_unswitch_pass.cpp",
        "SPIRV-Tools/source/opt/mem_pass.cpp",
        "SPIRV-Tools/source/opt/merge_return_pass.cpp",
        "SPIRV-Tools/source/opt/module.cpp",
        "SPIRV-Tools/source/opt/optimizer.cpp",
        "SPIRV-Tools/source/opt/pass.cpp",
        "SPIRV-Tools/source/opt/pass_manager.cpp",
        "SPIRV-Tools/source/opt/private_to_local_pass.cpp",
        "SPIRV-Tools/source/opt/propagator.cpp",
        "SPIRV-Tools/source/opt/reduce_load_size.cpp",
        "SPIRV-Tools/source/opt/redundancy_elimination.cpp",
        "SPIRV-Tools/source/opt/register_pressure.cpp",
        "SPIRV-Tools/source/opt/relax_float_ops_pass.cpp",
        "SPIRV-Tools/source/opt/remove_dontinline_pass.cpp",
        "SPIRV-Tools/source/opt/remove_duplicates_pass.cpp",
        "SPIRV-Tools/source/opt/remove_unused_interface_variables_pass.cpp",
        "SPIRV-Tools/source/opt/replace_desc_array_access_using_var_index.cpp",
        "SPIRV-Tools/source/opt/replace_invalid_opc.cpp",
        "SPIRV-Tools/source/opt/scalar_analysis.cpp",
        "SPIRV-Tools/source/opt/scalar_analysis_simplification.cpp",
        "SPIRV-Tools/source/opt/scalar_replacement_pass.cpp",
        "SPIRV-Tools/source/opt/set_spec_constant_default_value_pass.cpp",
        "SPIRV-Tools/source/opt/simplification_pass.cpp",
        "SPIRV-Tools/source/opt/spread_volatile_semantics.cpp",
        "SPIRV-Tools/source/opt/ssa_rewrite_pass.cpp",
        "SPIRV-Tools/source/opt/strength_reduction_pass.cpp",
        "SPIRV-Tools/source/opt/strip_debug_info_pass.cpp",
        "SPIRV-Tools/source/opt/strip_nonsemantic_info_pass.cpp",
        "SPIRV-Tools/source/opt/struct_cfg_analysis.cpp",
        "SPIRV-Tools/source/opt/type_manager.cpp",
        "SPIRV-Tools/source/opt/types.cpp",
        "SPIRV-Tools/source/opt/unify_const_pass.cpp",
        "SPIRV-Tools/source/opt/upgrade_memory_model.cpp",
        "SPIRV-Tools/source/opt/value_number_table.cpp",
        "SPIRV-Tools/source/opt/vector_dce.cpp",
        "SPIRV-Tools/source/opt/workaround1209.cpp",
        "SPIRV-Tools/source/opt/wrap_opkill.cpp",
        "SPIRV-Tools/source/opt/pch_source_opt.cpp",
	}

	includedirs
	{
        "SPIRV-Tools",
		"SPIRV-Tools/include",
		"SPIRV-Tools/build",
		"SPIRV-Headers/include",
	}


    filter "system:windows"
        defines {
            'WIN32',
            '_WINDOWS',
            'NDEBUG',
            '_CRT_SECURE_NO_WARNINGS',
            '_SCL_SECURE_NO_WARNINGS',
            'SPIRV_WINDOWS;SPIRV_COLOR_TERMINAL',
            'CMAKE_INTDIR="MinSizeRel"'
        }
	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"