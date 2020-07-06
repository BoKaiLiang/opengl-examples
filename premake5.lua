workspace "OpenGL"
	architecture "x64"
	startproject "normal_mapping"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"


include "ext"
include "core"
include "sandbox"