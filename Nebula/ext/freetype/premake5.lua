project "freetype"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/base/ftsystem.c",
        "src/base/ftlcdfil.c", 
        "src/base/ftstroke.c", 
        "src/base/ftbitmap.c", 
        "src/smooth/smooth.c", 
        "src/sfnt/sfnt.c", 
        "src/truetype/truetype.c", 
        "src/base/ftinit.c", 
        "src/base/ftdebug.c", 
        "src/base/ftbase.c", 
        "src/base/ftbbox.c",  
        "src/base/ftglyph.c",  
        "src/bdf/bdf.c",
        "src/winfonts/winfnt.c"
    }

    defines
    {
        "FT2_BUILD_LIBRARY",
        "_LIBS_SUPPLIED"
    }

    sysincludedirs
    {
        "include"
    }
    
    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"