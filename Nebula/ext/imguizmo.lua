project "imguizmo"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    

    location("build/%{prj.name}")
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imguizmo/*.cpp",
        "imguizmo/*.h"
    }

    externalincludedirs
    {
        "imguizmo",
        "imgui/"
    }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"