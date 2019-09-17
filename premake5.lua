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
glfwdir = "Prism/vendor/GLFW"

project "GLFW"
	location (glfwdir)
	kind "StaticLib"
	language "C"
	
	targetdir ("bin/" .. outputdir)
	objdir ("bin-int/%{prj.name}-" .. outputdir)
	
	files
	{
		(glfwdir .. "/include/GLFW/glfw3.h"),
		(glfwdir .. "/include/GLFW/glfw3native.h"),
		(glfwdir .. "/src/glfw_config.h"),
		(glfwdir .. "/src/context.c"),
		(glfwdir .. "/src/init.c"),
		(glfwdir .. "/src/input.c"),
		(glfwdir .. "/src/monitor.c"),
		(glfwdir .. "/src/vulkan.c"),
		(glfwdir .. "/src/window.c")
	}
	
	filter "system:windows"
		systemversion "latest"
		staticruntime "On"
		
		files
		{
			(glfwdir .. "/src/win32_init.c"),
			(glfwdir .. "/src/win32_joystick.c"),
			(glfwdir .. "/src/win32_monitor.c"),
			(glfwdir .. "/src/win32_time.c"),
			(glfwdir .. "/src/win32_thread.c"),
			(glfwdir .. "/src/win32_window.c"),
			(glfwdir .. "/src/wgl_context.c"),
			(glfwdir .. "/src/egl_context.c"),
			(glfwdir .. "/src/osmesa_context.c")
		}
		
		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}


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
		"%{prj.name}/vendor/GLFW/include",
		"vendor/spdlog/include",
		(os.getenv("VK_SDK_PATH") .. "/include"),
		(os.getenv("VK_SDK_PATH") .. "/shaderc/libshader/include")
	}
	
	dependson {
		"GLFW"
	}

	links {
		"GLFW", 
		(os.getenv("VK_SDK_PATH") .. "/lib/vulkan-1.lib"),
		(os.getenv("VK_SDK_PATH") .. "/lib/shaderc_combined.lib")
	}

	defines {
		"GLFW_INCLUDE_VULKAN"
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