project "imgui"
    kind "StaticLib"
    language "C"
    staticruntime "on"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imgui.cpp",
        "imgui_demo.cpp",
        "imgui_draw.cpp",
        "imgui_widgets.cpp",
        "examples/imgui_impl_glfw.cpp"
    }
    sysincludedirs
    {
        "./",
        "../../include"
    }
    
    filter "system:windows"
        systemversion "latest"
        files
        {
            "examples/imgui_impl_opengl3.cpp"
        }
    filter "system:linux"
        systemversion "latest"
        files
        {
            "examples/imgui_impl_opengl3.cpp"
        }
    filter "system:macosx"
        systemversion "latest"
        files
        {
            "examples/imgui_impl_metal.mm"
        }
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