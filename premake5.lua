workspace "Pagoda"

	configurations {
		"Debug",
		"Release",
		"Dist"
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

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/src/Pagoda",
		"%{prj.name}/vendor/spdlog/include",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"SPDLOG_COMPILED_LIB"
		}
		
	filter "system:macosx"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"SPDLOG_COMPILED_LIB"
		}


		--[[postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/TestApp")
		}--]]

	filter "configurations:Debug"
		defines {
		"PG_DEBUG",
		"PG_ENABLE_ASSERTS"
		}
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "PG_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Dist"
		defines "PG_DIST"
		buildoptions "/MD"
		optimize "On"

project "TestApp"
	location "TestApp"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Pagoda/vendor/spdlog/include",
		"Pagoda/src",
		"Pagoda/src/Pagoda"
	}
	
	--[[libdirs
	{
		"bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Pagoda",
		"Pagoda/vendor/spdlog/build/%{cfg.buildcfg}"
	}--]]
	
	commonLibDir = {}
	commonLibDir["Pagoda"] = "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Pagoda"

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"SPDLOG_COMPILED_LIB"
		}
	
	filter "system:macosx"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"SPDLOG_COMPILED_LIB"
		}

	filter "configurations:Debug"
		defines {
			"PG_ENABLE_ASSERTS",
			"PG_DEBUG"
		}
		buildoptions "/MDd"
		symbols "On"

		libdirs {
			"%{commonLibDir.Pagoda}",
			"Pagoda/vendor/spdlog/build/Debug"
		}

		links {
			"Pagoda.lib",
			"spdlogd.lib"
		}

	filter "configurations:Release"
		defines "PG_RELEASE"
		buildoptions "/MD"
		optimize "On"
		
		libdirs {
			"%{commonLibDir.Pagoda}",
			"Pagoda/vendor/spdlog/build/Release"
		}

		links {
			"Pagoda.lib",
			"spdlog.lib"
		}

	filter "configurations:Dist"
		defines "PG_DIST"
		buildoptions "/MD"
		optimize "On"
		
		libdirs {
			"%{commonLibDir.Pagoda}",
			"Pagoda/vendor/spdlog/build/Release"
		}

		links {
			"Pagoda.lib",
			"spdlog.lib"
		}