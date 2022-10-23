# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  glad_config = debug
  imgui_config = debug
  yaml_cpp_config = debug
  nfd_config = debug
  imguizmo_config = debug
  SPIRV_Cross_config = debug
  GenericCodeGen_config = debug
  OGLCompiler_config = debug
  OSDependent_config = debug
  MachineIndependent_config = debug
  SPIRV_config = debug
  SPVRemapper_config = debug
  glslang_config = debug
  glslang_default_resource_limits_config = debug
  spirv_tools_config = debug
  spirv_tools_opt_config = debug
  shaderc_util_config = debug
  shaderc_config = debug
  NebulaEngine_config = debug
  NebulaStudio_config = debug

else ifeq ($(config),release)
  glad_config = release
  imgui_config = release
  yaml_cpp_config = release
  nfd_config = release
  imguizmo_config = release
  SPIRV_Cross_config = release
  GenericCodeGen_config = release
  OGLCompiler_config = release
  OSDependent_config = release
  MachineIndependent_config = release
  SPIRV_config = release
  SPVRemapper_config = release
  glslang_config = release
  glslang_default_resource_limits_config = release
  spirv_tools_config = release
  spirv_tools_opt_config = release
  shaderc_util_config = release
  shaderc_config = release
  NebulaEngine_config = release
  NebulaStudio_config = release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := glad imgui yaml-cpp nfd imguizmo SPIRV-Cross GenericCodeGen OGLCompiler OSDependent MachineIndependent SPIRV SPVRemapper glslang glslang-default-resource-limits spirv-tools spirv-tools-opt shaderc_util shaderc NebulaEngine NebulaStudio

.PHONY: all clean help $(PROJECTS) Dependencies

all: $(PROJECTS)

Dependencies: GenericCodeGen MachineIndependent OGLCompiler OSDependent SPIRV SPIRV-Cross SPVRemapper glad glslang glslang-default-resource-limits imgui imguizmo nfd shaderc shaderc_util spirv-tools spirv-tools-opt yaml-cpp

glad:
ifneq (,$(glad_config))
	@echo "==== Building glad ($(glad_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/glad -f Makefile config=$(glad_config)
endif

imgui:
ifneq (,$(imgui_config))
	@echo "==== Building imgui ($(imgui_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/imgui -f Makefile config=$(imgui_config)
endif

yaml-cpp:
ifneq (,$(yaml_cpp_config))
	@echo "==== Building yaml-cpp ($(yaml_cpp_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/yaml-cpp -f Makefile config=$(yaml_cpp_config)
endif

nfd:
ifneq (,$(nfd_config))
	@echo "==== Building nfd ($(nfd_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/nativefiledialog -f Makefile config=$(nfd_config)
endif

imguizmo:
ifneq (,$(imguizmo_config))
	@echo "==== Building imguizmo ($(imguizmo_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/imguizmo -f Makefile config=$(imguizmo_config)
endif

SPIRV-Cross:
ifneq (,$(SPIRV_Cross_config))
	@echo "==== Building SPIRV-Cross ($(SPIRV_Cross_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/SPIRV-Cross/SPIRV-Cross -f Makefile config=$(SPIRV_Cross_config)
endif

GenericCodeGen:
ifneq (,$(GenericCodeGen_config))
	@echo "==== Building GenericCodeGen ($(GenericCodeGen_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/GenericCodeGen -f Makefile config=$(GenericCodeGen_config)
endif

OGLCompiler:
ifneq (,$(OGLCompiler_config))
	@echo "==== Building OGLCompiler ($(OGLCompiler_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/OGLCompiler -f Makefile config=$(OGLCompiler_config)
endif

OSDependent:
ifneq (,$(OSDependent_config))
	@echo "==== Building OSDependent ($(OSDependent_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/OSDependent -f Makefile config=$(OSDependent_config)
endif

MachineIndependent: GenericCodeGen OGLCompiler OSDependent
ifneq (,$(MachineIndependent_config))
	@echo "==== Building MachineIndependent ($(MachineIndependent_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/MachineIndependent -f Makefile config=$(MachineIndependent_config)
endif

SPIRV: GenericCodeGen OGLCompiler OSDependent MachineIndependent
ifneq (,$(SPIRV_config))
	@echo "==== Building SPIRV ($(SPIRV_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/SPIRV -f Makefile config=$(SPIRV_config)
endif

SPVRemapper:
ifneq (,$(SPVRemapper_config))
	@echo "==== Building SPVRemapper ($(SPVRemapper_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/SPVRemapper -f Makefile config=$(SPVRemapper_config)
endif

glslang:
ifneq (,$(glslang_config))
	@echo "==== Building glslang ($(glslang_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/glslang -f Makefile config=$(glslang_config)
endif

glslang-default-resource-limits:
ifneq (,$(glslang_default_resource_limits_config))
	@echo "==== Building glslang-default-resource-limits ($(glslang_default_resource_limits_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/glslang-default-resource-limits -f Makefile config=$(glslang_default_resource_limits_config)
endif

spirv-tools:
ifneq (,$(spirv_tools_config))
	@echo "==== Building spirv-tools ($(spirv_tools_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/spirv-tools -f Makefile config=$(spirv_tools_config)
endif

spirv-tools-opt: spirv-tools
ifneq (,$(spirv_tools_opt_config))
	@echo "==== Building spirv-tools-opt ($(spirv_tools_opt_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/spirv-tools-opt -f Makefile config=$(spirv_tools_opt_config)
endif

shaderc_util: GenericCodeGen OGLCompiler OSDependent MachineIndependent SPIRV SPVRemapper glslang glslang-default-resource-limits spirv-tools spirv-tools-opt
ifneq (,$(shaderc_util_config))
	@echo "==== Building shaderc_util ($(shaderc_util_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/shaderc_util -f Makefile config=$(shaderc_util_config)
endif

shaderc: shaderc_util
ifneq (,$(shaderc_config))
	@echo "==== Building shaderc ($(shaderc_config)) ===="
	@${MAKE} --no-print-directory -C Nebula/ext/build/shaderc -f Makefile config=$(shaderc_config)
endif

NebulaEngine: imgui imguizmo glad yaml-cpp nfd SPIRV-Cross GenericCodeGen OGLCompiler OSDependent MachineIndependent SPIRV SPVRemapper glslang glslang-default-resource-limits spirv-tools spirv-tools-opt shaderc_util shaderc
ifneq (,$(NebulaEngine_config))
	@echo "==== Building NebulaEngine ($(NebulaEngine_config)) ===="
	@${MAKE} --no-print-directory -C build -f NebulaEngine.make config=$(NebulaEngine_config)
endif

NebulaStudio: NebulaEngine imgui imguizmo glad yaml-cpp nfd SPIRV-Cross
ifneq (,$(NebulaStudio_config))
	@echo "==== Building NebulaStudio ($(NebulaStudio_config)) ===="
	@${MAKE} --no-print-directory -C build -f NebulaStudio.make config=$(NebulaStudio_config)
endif

clean:
	@${MAKE} --no-print-directory -C Nebula/ext/glad -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/imgui -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/yaml-cpp -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/nativefiledialog -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/imguizmo -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/SPIRV-Cross/SPIRV-Cross -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/GenericCodeGen -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/OGLCompiler -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/OSDependent -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/MachineIndependent -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/SPIRV -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/SPVRemapper -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/glslang -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/glslang-default-resource-limits -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/spirv-tools -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/spirv-tools-opt -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/shaderc_util -f Makefile clean
	@${MAKE} --no-print-directory -C Nebula/ext/build/shaderc -f Makefile clean
	@${MAKE} --no-print-directory -C build -f NebulaEngine.make clean
	@${MAKE} --no-print-directory -C build -f NebulaStudio.make clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   glad"
	@echo "   imgui"
	@echo "   yaml-cpp"
	@echo "   nfd"
	@echo "   imguizmo"
	@echo "   SPIRV-Cross"
	@echo "   GenericCodeGen"
	@echo "   OGLCompiler"
	@echo "   OSDependent"
	@echo "   MachineIndependent"
	@echo "   SPIRV"
	@echo "   SPVRemapper"
	@echo "   glslang"
	@echo "   glslang-default-resource-limits"
	@echo "   spirv-tools"
	@echo "   spirv-tools-opt"
	@echo "   shaderc_util"
	@echo "   shaderc"
	@echo "   NebulaEngine"
	@echo "   NebulaStudio"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"