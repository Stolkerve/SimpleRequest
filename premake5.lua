workspace "SimpleRequest"  
	startproject "SimpleRequest"

	configurations
	{
		"Debug",
		"Release"
	}

	flags
	{
		"MultiProcessorCompile"
	}

IncludeDir = {}
IncludeDir["glfw"] = "%{wks.location}/SimpleRequest/dependencies/glfw/include"
IncludeDir["glad"] = "%{wks.location}/SimpleRequest/dependencies/glad/include"
IncludeDir["imgui"] = "%{wks.location}/SimpleRequest/dependencies/imgui"
IncludeDir["ImGuiColorTextEdit"] = "%{wks.location}/SimpleRequest/dependencies/ImGuiColorTextEdit"

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

group "Dependencies"
	include "SimpleRequest/dependencies/glfw"
	include "SimpleRequest/dependencies/glad"
group ""
	include "SimpleRequest"
