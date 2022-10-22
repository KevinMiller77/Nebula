-- Native file dialog premake5 script
--
-- This can be ran directly, but commonly, it is only run
-- by package maintainers.
--
-- IMPORTANT NOTE: premake5 alpha 9 does not handle this script
-- properly.  Build premake5 from Github master, or, presumably,
-- use alpha 10 in the future.

src_dir = "../nativefiledialog/src"
build_dir = src_dir .. "/build"

target_dir = "../lib/"

project "nfd"
  kind "StaticLib"
  language "C++"
  cppdialect "C++17"

  cmake_level = "Release"
  filter "configurations:Debug"
      cmake_level = "Debug"   

  targetdir ("../lib/" .. cmake_level)
  objdir (build_dir)
  location(build_dir)
  
  includedirs {src_dir .. "/include/"}
  
  -- common files
  files {
    src_dir .. "/*.h",
    src_dir .. "/include/*.h",
    src_dir .. "/nfd_common.c",
  }

  -- debug/release filters
  filter "configurations:Debug"
    defines {"DEBUG"}
    symbols "On"

  filter "configurations:Release"
    defines {"NDEBUG"}
    optimize "On"
  
  -- system build filters
  filter "system:windows"
    language "C++"
    files {src_dir .. "/nfd_win.cpp"}
    links { "user32" }
  
  filter {"action:gmake or action:xcode4"}
    buildoptions {"-fno-exceptions"}
  
  filter "system:macosx"
    language "C"
    files {src_dir .. "/nfd_cocoa.m"}
  
  filter {"system:linux"}
    language "C"
    files {src_dir .. "/nfd_zenity.c"}
  
  
  -- visual studio filters
  filter "action:vs*"
    defines { "_CRT_SECURE_NO_WARNINGS" }  