project "MachineIndependent"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
    staticruntime("On")

    location("../../build/%{prj.name}")
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../bin-int/" .. outputdir .. "/%{prj.name}")

    dependson {
        "GenericCodeGen",
        "OGLCompiler",
        "OSDependent",
    }

    p = path.getabsolute(".")

    prebuildcommands {
        "{MKDIR} " .. p .. "/glslang/build",
        "cmake -D SKIP_GLSLANG_INSTALL=ON -S " .. p .. "/glslang -B " .. p .. "/glslang/build"
    }

	files
	{
        "glslang/glslang/MachineIndependent/glslang_tab.cpp",
        "glslang/glslang/MachineIndependent/attribute.cpp",
        "glslang/glslang/MachineIndependent/Constant.cpp",
        "glslang/glslang/MachineIndependent/iomapper.cpp",
        "glslang/glslang/MachineIndependent/InfoSink.cpp",
        "glslang/glslang/MachineIndependent/Initialize.cpp",
        "glslang/glslang/MachineIndependent/IntermTraverse.cpp",
        "glslang/glslang/MachineIndependent/Intermediate.cpp",
        "glslang/glslang/MachineIndependent/ParseContextBase.cpp",
        "glslang/glslang/MachineIndependent/ParseHelper.cpp",
        "glslang/glslang/MachineIndependent/PoolAlloc.cpp",
        "glslang/glslang/MachineIndependent/RemoveTree.cpp",
        "glslang/glslang/MachineIndependent/Scan.cpp",
        "glslang/glslang/MachineIndependent/ShaderLang.cpp",
        "glslang/glslang/MachineIndependent/SpirvIntrinsics.cpp",
        "glslang/glslang/MachineIndependent/SymbolTable.cpp",
        "glslang/glslang/MachineIndependent/Versions.cpp",
        "glslang/glslang/MachineIndependent/intermOut.cpp",
        "glslang/glslang/MachineIndependent/limits.cpp",
        "glslang/glslang/MachineIndependent/linkValidate.cpp",
        "glslang/glslang/MachineIndependent/parseConst.cpp",
        "glslang/glslang/MachineIndependent/reflection.cpp",
        "glslang/glslang/MachineIndependent/preprocessor/Pp.cpp",
        "glslang/glslang/MachineIndependent/preprocessor/PpAtom.cpp",
        "glslang/glslang/MachineIndependent/preprocessor/PpContext.cpp",
        "glslang/glslang/MachineIndependent/preprocessor/PpScanner.cpp",
        "glslang/glslang/MachineIndependent/preprocessor/PpTokens.cpp",
        "glslang/glslang/MachineIndependent/propagateNoContraction.cpp",
        "glslang/glslang/HLSL/hlslAttributes.cpp",
        "glslang/glslang/HLSL/hlslParseHelper.cpp",
        "glslang/glslang/HLSL/hlslScanContext.cpp",
        "glslang/glslang/HLSL/hlslOpMap.cpp",
        "glslang/glslang/HLSL/hlslTokenStream.cpp",
        "glslang/glslang/HLSL/hlslGrammar.cpp",
        "glslang/glslang/HLSL/hlslParseables.cpp",
	}

	includedirs
	{
        "glslang/build/include",
        "glslang/glslang/MachineIndependent",
        "glslang/glslang/HLSL"
	}

    defines {
        "ENABLE_HLSL"
    }

    filter "system:windows"
        defines {
            'WIN32',
            '_WINDOWS',
        }
        
	filter "system:linux"
		pic "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"