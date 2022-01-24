workspace "Pagoda"

	configurations {
		"Debug",
		"Release",
	}

	platforms {
		"Win32",
		"Win64",
		"WinARM64",
		"MacOS",
	}
	
	filter { "platforms:Win32" }
		system "windows"
		architecture "x86"
		defines "PG_PLATFORM_WINDOWS"

	filter { "platforms:Win64" }
		system "windows"
		architecture "x64"
		defines "PG_PLATFORM_WINDOWS"

	filter { "platforms:WinARM64" }
		system "windows"
		architecture "arm64"
		defines "PG_PLATFORM_WINDOWS"

	filter { "platforms:MacOS" }
		system "macosx"
		architecture "arm64"
		defines "PG_PLATFORM_MACOS"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Pagoda"
	location "Pagoda"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pgpch.h"
	pchsource "Pagoda/src/Pagoda/pgpch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines {
		"SPDLOG_COMPILED_LIB"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/src/Pagoda",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glfw/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

	filter "system:macosx"
		cppdialect "C++17"
		systemversion "latest"

		--[[postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/TestApp")
		}--]]

	filter "configurations:Debug"
		defines {
		"PG_DEBUG",
		"PG_ENABLE_ASSERTS"
		}
		staticruntime "Off"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PG_RELEASE"
		staticruntime "Off"
		runtime "Release"
		optimize "On"

project "TestApp"
	location "TestApp"
	kind "ConsoleApp"
	language "C++"

	cppdialect "C++17"
	systemversion "latest"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines {
		"SPDLOG_COMPILED_LIB"
	}

	includedirs {
		"Pagoda/src",
		"Pagoda/src/Pagoda",
		"Pagoda/vendor/spdlog/include",
		"Pagoda/vendor/glfw/include",
	}

	libdirs {
		"bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Pagoda",
		"Pagoda/vendor/spdlog/build"
	}

	links {
		"Pagoda.lib",
		"glfw3.lib"
	}
	
	--[[libdirs
	{
		"bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Pagoda",
		"Pagoda/vendor/spdlog/build/%{cfg.buildcfg}"
	}--]]

	filter "configurations:Debug"
		defines {
			"PG_ENABLE_ASSERTS",
			"PG_DEBUG"
		}
		staticruntime "Off"
		runtime "Debug"
		symbols "On"

		libdirs {
			"Pagoda/vendor/spdlog/build/Debug",
			"Pagoda/vendor/glfw/src/Debug"
		}

	filter "configurations:Release"
		defines "PG_RELEASE"
		staticruntime "Off"
		runtime "Release"
		optimize "On"
		
		libdirs {
			"Pagoda/vendor/spdlog/build/Release",
			"Pagoda/vendor/glfw/src/Release"
		}

	filter "system:windows"
		filter "configurations:Debug"
			links "spdlogd.lib"

		filter "configurations:Release"
			links "spdlog.lib"
		
		filter "system:macosx"
			links "libspdlog.a"