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
		buildoptions {
			"-mmacosx-version-min=11.0"
		}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Pagoda"
	location "Pagoda"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	defines {
		"SPDLOG_COMPILED_LIB"
	}

	incDirs = {
		"%{prj.name}/src",
		"%{prj.name}/src/Pagoda",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glfw/include",
		"%{prj.name}/vendor/eigen",
		"%{prj.name}/vendor/glad/include",
		"%{prj.name}/vendor/lua/lua/include"
	}

	includedirs {
		table.unpack(incDirs)
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		pchheader "pgpch.h"
		pchsource "Pagoda/src/Pagoda/pgpch.cpp"

	filter "system:macosx"
		cppdialect "C++17"
		systemversion "latest"

		sysincludedirs {
			table.unpack(incDirs)
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

	incDirs = {
		"Pagoda/src",
		"Pagoda/src/Pagoda",
		"Pagoda/vendor/spdlog/include",
		"Pagoda/vendor/glfw/include",
		"Pagoda/vendor/eigen",
		"Pagoda/vendor/glad/include",
		"Pagoda/vendor/lua/lua/include"
	}

	includedirs {
		table.unpack(incDirs)
	}

	libdirs {
		"bin/%{outputdir}/Pagoda",
		"Pagoda/vendor/lua/bin/%{outputdir}/lua",
		"Pagoda/vendor/lua/bin",
		"Pagoda/vendor/spdlog/build",
		"Pagoda/vendor/glfw/build/src"
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

	filter "configurations:Release"
		defines "PG_RELEASE"
		staticruntime "Off"
		runtime "Release"
		optimize "On"
	
	filter "system:macosx"
	 sysincludedirs {
		 table.unpack(incDirs)
	 }

	filter {"system:windows", "configurations:Debug"}
		libdirs {
			"Pagoda/vendor/spdlog/build/Debug",
			"Pagoda/vendor/glfw/src/Debug"
		}

		links {
			"Pagoda.lib",
			"spdlogd.lib",
			"glfw3.lib",
			"OpenGL32.lib",
			"lua.lib"
		}

	filter {"system:windows", "configurations:Release"}
	libdirs {
		"Pagoda/vendor/spdlog/build/Release",
		"Pagoda/vendor/glfw/src/Release"
	}

		links {
			"Pagoda.lib",
			"spdlog.lib",
			"glfw3.lib",
			"OpenGL32.lib",
			"lua.lib"
		}
		
	filter "system:macosx"
		links {
			"OpenGL.framework",
			"CoreFoundation.framework", 
			"Cocoa.framework",
	 		"IOKit.framework"
		}
		links {
			"spdlog",
			"Pagoda",
			"glfw3",
			"lua"
		}