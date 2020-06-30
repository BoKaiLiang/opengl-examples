-- External library
project "external"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin/intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		-- glad
		"glad/include/glad/glad.h",
		"glad/include/KHR/khrplatform.h",
		"glad/src/glad.c",

		-- glfw3
		"glfw3/include/GLFW/glfw3.h",
		"glfw3/include/GLFW/glfw3native.h",
		"glfw3/src/glfw_config.h",
		"glfw3/src/context.c",
		"glfw3/src/init.c",
		"glfw3/src/input.c",
		"glfw3/src/monitor.c",
		"glfw3/src/vulkan.c",
		"glfw3/src/window.c",

		-- Dear imgui
		"imgui/imconfig.h",
		"imgui/imgui.h",
		"imgui/imgui.cpp",
		"imgui/imgui_draw.cpp",
		"imgui/imgui_internal.h",
		"imgui/imgui_widgets.cpp",
		"imgui/imstb_rectpack.h",
		"imgui/imstb_textedit.h",
		"imgui/imstb_truetype.h",
		"imgui/imgui_demo.cpp",

		-- spdlog
		"spdlog/include/spdlog/**.h",
		"spdlog/include/spdlog/cfg/**.h",
		"spdlog/include/spdlog/details/**.h",
		"spdlog/include/spdlog/fmt/**.h",
		"spdlog/include/spdlog/sinks/**.h",
		"spdlog/src/**.cpp",

		-- stb
		"stb/**.h",
		"stb/**.cpp",

		-- tinyobjloader
		"tinyobjloader/**.h",
		"tinyobjloader/**.cpp"
	}

	links 
	{ 
		"opengl32.lib"
	}

	includedirs
    {
		-- For glad.c directory
        "glad/include",
		"spdlog/include"
    }

	defines
	{
		"SPDLOG_COMPILED_LIB"
	}

	filter "system:windows"
		systemversion "latest"

		files
		{
			"glfw3/src/win32_init.c",
			"glfw3/src/win32_joystick.c",
			"glfw3/src/win32_monitor.c",
			"glfw3/src/win32_time.c",
			"glfw3/src/win32_thread.c",
			"glfw3/src/win32_window.c",
			"glfw3/src/wgl_context.c",
			"glfw3/src/egl_context.c",
			"glfw3/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
