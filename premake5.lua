include "locations.lua"

workspace "AlphaShrink"

configurations { "Debug", "Release", "Distribution" }
    platforms { "Win64", "MacOSX" }
    startproject "Sandbox"
    language "C++"
    cppdialect "C++20"
    flags { "MultiProcessorCompile" }
    targetdir ("bin/" .. outputdir)
    objdir ("bin-int/" .. outputdir)

    --glm
    includedirs { thirdpartiesdir .. "/glm" }
    --json
    includedirs { thirdpartiesdir .. "/json/include"}
      
    filter "configurations:Debug"
        includedirs { thirdpartiesdir .. "/SimpleTests/include" }
        defines { "MANI_DEBUG" }
        defines { "MANI_PROJECTROOT_PATH=\"".. _MAIN_SCRIPT_DIR .. "\"" }
        symbols "On"
    
    filter "configurations:Release"
        defines { "MANI_RELEASE" }
        defines { "MANI_PROJECTROOT_PATH=\"".. _MAIN_SCRIPT_DIR .. "\"" }
        optimize "On"

    filter "configurations:Distribution"
        defines { "MANI_DISTRIBUTION" }
        optimize "On"

    filter "platforms:MacOS"
        architecture "x64"
        system "macosx"

    filter "platforms:Win64"
        architecture "x64"
        system "windows"
    
    filter "system:macosx"
        defines { "MANI_MACOSX" }
            
    filter "system:windows"
        defines { "MANI_WINDOWS" }

group "Engine"
    include "Engine"

group ""
-- Executables
project "AlphaShrink"
    kind "ConsoleApp"
    location "%{prj.name}"

    files { "%{prj.name}/Sources/**.h", "%{prj.name}/Sources/**.cpp" }

    links { "Core", "OpenGL", "ECS", "Camera", "FloatingCamera", "Assets", "RenderAPI", "Scene" }

    includedirs { moduledir .. "/**", "%{prj.name}/Sources/" }
