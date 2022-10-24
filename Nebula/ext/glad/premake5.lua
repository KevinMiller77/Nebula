project "glad"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "../../include/glad/glad.h",
        "../../include/KHR/khrplatform.h",
        "src/glad.c"
    }

    externalincludedirs
    {
        "../../include"
    }
    
    filter "system:windows"
        

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"