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
            "%{prj.name}/Core.h",
            "%{prj.name}/res/**",
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp",
            "%{prj.name}/vendor/**.h",
            "%{prj.name}/vendor/**.cpp"
        }

        includedirs {
            "%{prj.name}/src",
            "%{prj.name}/vendor/spdlog/include",
            "GLEW/include",
            "GLFW/include",
            "ImGui"
        }

        links {
            "GLEW",
            "GLFW",
            "ImGui",
            "Opengl32"
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
        kind "WindowedApp"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs {
            "%{wks.location.relpath}/Core/src",
            "%{wks.location.relpath}/Core/vendor/spdlog/include",
            "%{wks.location.relpath}/GLEW/include"
        }

        links {
            "Core",
            "Opengl32"
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


    project "ImGui"
        location "ImGui"
        kind "SharedLib"
        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

        files {
            "%{prj.name}/imconfig.h",
            "%{prj.name}/imgui.h",
            "%{prj.name}/imgui.cpp",
            "%{prj.name}/imgui_draw.cpp",
            "%{prj.name}/imgui_internal.h",
            "%{prj.name}/imgui_widgets.cpp",
            "%{prj.name}/examples/imgui_impl_glfw.h",
            "%{prj.name}/examples/imgui_impl_glfw.cpp",
            "%{prj.name}/examples/imgui_impl_opengl3.h",
            "%{prj.name}/examples/imgui_impl_opengl3.cpp"
        }

        includedirs {
            "%{prj.name}",
            "GLEW/include",
            "GLFW/include"
        }

        links {
            "GLEW",
            "GLFW",
            "Opengl32"
        }

        defines {
            "_IMGUI_BUILD_DLL",
            "IMGUI_IMPL_OPENGL_LOADER_GLEW"
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