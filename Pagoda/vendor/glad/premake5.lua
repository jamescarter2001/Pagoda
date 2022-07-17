workspace "glad"

	configurations {
		"Debug",
		"Release",
	}

	platforms {
		"Win32",
		"Win64",
		"Linux",
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

	filter { "platforms:Linux" }
		system "linux"
		architecture "x64"
		defines "PG_PLATFORM_LINUX"

	filter { "platforms:MacOS" }
		system "macosx"
		architecture "arm64"
		defines "PG_PLATFORM_MACOS"
		buildoptions {
			"-mmacosx-version-min=11.0"
		}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "glad"
	location "glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.c"
	}

	includedirs {
		"%{prj.name}/include",
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
	
	filter "system:linux"
		cppdialect "C++17"
		systemversion "latest"

	filter "system:macosx"
		cppdialect "C++17"
		systemversion "latest"

	filter "configurations:Debug"
		staticruntime "Off"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "PG_RELEASE"
		staticruntime "Off"
		runtime "Release"
		optimize "On"