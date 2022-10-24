project "imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imgui/imgui.cpp",
        "imgui/imgui_demo.cpp",
        "imgui/imgui_draw.cpp",
        "imgui/imgui_widgets.cpp",
        "imgui/imgui_tables.cpp"
    }
    externalincludedirs
    {
        "imgui",
        "../include"
    }
    
    filter "system:windows"
        
    filter "system:linux"
        
    filter "system:macosx"
        
        buildoptions
        {
            "-x objective-c++"
        }

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"