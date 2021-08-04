project "SimpleRequest"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.hh",
		"src/**.cpp",
		"dependencies/ImGuiColorTextEdit/**.cpp",
		"dependencies/ImGuiColorTextEdit/**.h",
		"dependencies/imgui/**.h",
		"dependencies/imgui/**.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE",
	}

	includedirs
	{
		"src",
		"dependencies/",
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.ImGuiColorTextEdit}",
	}

	filter "configurations:Debug"
		libdirs {
			"dependencies/glfw/bin/%{cfg.buildcfg}-%{cfg.system}/glfw/",
			"dependencies/glad/bin/%{cfg.buildcfg}-%{cfg.system}/glad/",
		}

	filter "configurations:Release"
		libdirs {
			"dependencies/glfw/bin/%{cfg.buildcfg}-%{cfg.system}/glfw/",
			"dependencies/glad/bin/%{cfg.buildcfg}-%{cfg.system}/glad/",
		}

	filter "system:linux"
		pic "On"
		systemversion "latest"
		links
		{
			"X11",
			"dl",
			"pthread",
			"glfw",
			"glad",
			"GL",
		}
	filter "system:windows"
		systemversion "latest"

		links
		{
			"glfw",
			"glad",
			"opengl32.lib",
			"ws2_32.lib"
		}

	filter "configurations:Debug"
		defines "SIMPLEREQUEST_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SIMPLEREQUEST_RELEASE"
		runtime "Release"
		optimize "on"