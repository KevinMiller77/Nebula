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

  includedirs {"src/include/"}

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  filter "system:macosx"
      platforms {"x64"}
    filter "system:windows or system:linux"
      platforms {"x64", "x86"}

    -- architecture filters
    filter "configurations:x86"
      architecture "x86"

    filter "configurations:x64"
      architecture "x86_64"

    -- debug/release filters
    filter "configurations:Debug"
      defines {"DEBUG"}
      symbols "On"
      targetsuffix "_d"

    filter "configurations:Release"
      defines {"NDEBUG"}
      optimize "On"

    -- common files
    files {"src/*.h",
            "src/include/*.h",
            "src/nfd_common.c",
    }


    warnings "extra"

    -- system build filters
    filter "system:windows"
      language "C++"
      files {"src/nfd_win.cpp"}

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
    