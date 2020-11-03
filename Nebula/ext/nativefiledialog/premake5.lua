-- Native file dialog premake5 script
--
-- This can be ran directly, but commonly, it is only run
-- by package maintainers.
--
-- IMPORTANT NOTE: premake5 alpha 9 does not handle this script
-- properly.  Build premake5 from Github master, or, presumably,
-- use alpha 10 in the future.

project "nfd"
  kind "StaticLib"
  
  outputdir = "%{cfg.buildcfg}/%{cfg.system}%{cfg.architecture}"
  
  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
  
  includedirs {"src/include/"}
  
  -- common files
  files {
    "src/*.h",
    "src/include/*.h",
    "src/nfd_common.c",
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
    files {"src/nfd_win.cpp"}
    links { "user32" }
  
  filter {"action:gmake or action:xcode4"}
    buildoptions {"-fno-exceptions"}
  
  filter "system:macosx"
    language "C"
    files {"src/nfd_cocoa.m"}
  
  filter {"system:linux", "options:linux_backend=gtk3"}
    language "C"
    files {"src/nfd_gtk.c"}
    buildoptions {"`pkg-config --cflags gtk+-3.0`"}
  filter {"system:linux", "options:linux_backend=zenity"}
    language "C"
    files {"src/nfd_zenity.c"}
  
  
  -- visual studio filters
  filter "action:vs*"
    defines { "_CRT_SECURE_NO_WARNINGS" }      
