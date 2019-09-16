workspace "Prism"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release"
	}

	defines {
		"NOMINMAX"
	}

outputdir = "%{cfg.buildcfg}"

project "Prism"
	location "Prism"
	kind "StaticLib"
	language "C++"
	
	targetdir ("bin/" .. outputdir)
	objdir ("bin-int/%{prj.name}-" .. outputdir)

	pchheader "pch.h"
	pchsource "Prism/src/pch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"vendor/spdlog/include",
		(os.getenv("VK_SDK_PATH") .. "/include"),
		(os.getenv("VK_SDK_PATH") .. "/shaderc/libshader/include")
	}

	links {
		(os.getenv("VK_SDK_PATH") .. "/lib/vulkan-1.lib"),
		(os.getenv("VK_SDK_PATH") .. "/lib/shaderc_combined.lib")
	}

	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"
	
	filter "configurations:Debug"
		defines "PR_DEBUG"
		buildoptions "/MT" -- to not conflict with shaderc
		symbols "On"

	filter "configurations:Release"
		defines "PR_RELEASE"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir)
	objdir ("bin-int/%{prj.name}-" .. outputdir)

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"Prism/src",
		"vendor/spdlog/include"
	}

	dependson {
		"Prism"
	}
	
	links {
		"Prism"
	}

	cppdialect "C++17"
	staticruntime "On"
	systemversion "latest"
	
	filter "configurations:Debug"
		defines "PR_DEBUG"
		buildoptions "/MT" -- to not conflict with shaderc
		symbols "On"

	filter "configurations:Release"
		defines "PR_RELEASE"
		optimize "On"