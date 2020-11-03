project "imgui"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imgui.cpp",
        "imgui_demo.cpp",
        "imgui_draw.cpp",
        "imgui_widgets.cpp"
    }
    sysincludedirs
    {
        "./",
        "../../include"
    }
    
    filter "system:windows"
        systemversion "latest"
    filter "system:linux"
        systemversion "latest"
    filter "system:macosx"
        systemversion "latest"
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