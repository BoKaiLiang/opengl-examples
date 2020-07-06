-- Core, utilities
project "core"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin/intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"**.h",
		"**.cpp"
	}

	includedirs
	{
		"../ext/glad/include",
		"../ext/glfw3/include",
		"../ext/glm/include",
		"../ext/spdlog/include",
		"../ext/imgui",
		"../ext/stb",
		"../ext/tinyobjloader",
		"../ext/assimp/include"
	}

	links
	{
		"external"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

		links
		{
			"../ext/assimp/lib/Debug/assimp-vc142-mtd.lib"
		}

	filter "configurations:Release"
		runtime "Release"
        optimize "on"