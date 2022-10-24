project "nfd"
  kind "StaticLib"
  
  outputdir = "%{cfg.buildcfg}/%{cfg.system}%{cfg.architecture}"
  
  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
  
  includedirs {"nfd/src/include/"}
  
  -- common files
  files {
    "nfd/src/*.h",
    "nfd/src/include/*.h",
    "nfd/src/nfd_common.c",
  }

  -- debug/release filters
  filter "configurations:Debug"
    defines {"DEBUG"}
    symbols "On"
    targetsuffix "_d"

  filter "configurations:Release"
    defines {"NDEBUG"}
    optimize "On"
  
  -- system build filters
  filter "system:windows"
    language "C++"
    files {"nfd/src/nfd_win.cpp"}
    links { "user32" }
  
  filter {"action:gmake or action:xcode4"}
    buildoptions {"-fno-exceptions"}
  
  filter "system:macosx"
    language "C"
    files {"nfd/src/nfd_cocoa.m"}
  
  filter {"system:linux"}
    language "C"
    files {"nfd/src/nfd_zenity.c"}
  
  
  -- visual studio filters
  filter "action:vs*"
    defines { "_CRT_SECURE_NO_WARNINGS" }      
