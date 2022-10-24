project "freetype-gl"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    externalincludedirs
    {
        "include",
        "../freetype/include",
        "../../include"
    }

    files
    {
        "vector.c",
        "texture-atlas.c",
        "texture-font.c"
    }
    
    filter "system:windows"
        

    filter "system:linux"
        

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"