require("premake", ">=5.0.0-alpha13")

workspace "DAT205"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

    outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"


    project "Core"
        location "Core"
        kind "SharedLib"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        pchheader "pch.h"
	    pchsource "Core/src/pch.cpp"

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs {
            "%{prj.name}/src"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"

            defines {
                "CORE_BUILD_DLL"
            }

            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
            }

        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:Release"
            defines { "NDEBUG" }
            optimize "On"

        filter "configurations:Dist"
            defines { "NDEBUG" }
            optimize "Full"


    project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs {
            "%{wks.location.relpath}/Core/src"
        }

        links {
            "Core"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"

        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:Release"
            defines { "NDEBUG" }
            optimize "On"

        filter "configurations:Dist"
            defines { "NDEBUG" }
            optimize "Full"


    project "GLEW"
        location "GLEW"
        kind "SharedLib"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/include/**.h",
            "%{prj.name}/src/glew.c"
        }

        includedirs {
            "%{prj.name}/include"
        }

        libdirs {
            os.findlib("Opengl32.lib")
        }

        links {
            "Opengl32"
        }

        defines {
            "GLEW_BUILD"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"

            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
            }

        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:Release"
            defines { "NDEBUG" }
            optimize "On"

        filter "configurations:Dist"
            defines { "NDEBUG" }
            optimize "Full"


    project "GLFW"
        location "GLFW"
        kind "SharedLib"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            --"%{prj.name}/deps/**.h",
            --"%{prj.name}/deps/**.c",
            "%{prj.name}/include/**.h",
            "%{prj.name}/src/context.c",
            "%{prj.name}/src/egl_*",
            "%{prj.name}/src/init.c",
            "%{prj.name}/src/input.c",
            "%{prj.name}/src/internal.h",
            "%{prj.name}/src/monitor.c",
            "%{prj.name}/src/vulkan.c",
            "%{prj.name}/src/wgl_context.h",
            "%{prj.name}/src/wgl_context.c",
            "%{prj.name}/src/win32*",
            "%{prj.name}/src/window.c"
        }

        defines {
            "_GLFW_WIN32",
            "_GLFW_BUILD_DLL"
        }

        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"

            postbuildcommands
            {
                ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
            }

        filter "configurations:Debug"
            defines { "DEBUG" }
            symbols "On"

        filter "configurations:Release"
            defines { "NDEBUG" }
            optimize "On"

        filter "configurations:Dist"
            defines { "NDEBUG" }
            optimize "Full"