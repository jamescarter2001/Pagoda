workspace "Pagoda"

	configurations {
		"Debug",
		"Release",
	}

	platforms {
		"WinARM64",
		"Win64",
		"Linux",
		"MacOS",
	}
	
	filter { "platforms:WinARM64" }
		system "windows"
		architecture "arm64"
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

project "Pagoda"
	location "Pagoda"
	kind "StaticLib"
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
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/glfw/include",
		"%{prj.name}/vendor/eigen",
		"%{prj.name}/vendor/glad/glad/include",
		"%{prj.name}/vendor/lua/lua/include"
	}

	includedirs {
		table.unpack(incDirs)
	}

	filter "system:windows"
		pchheader "pgpch.h"
		pchsource "Pagoda/src/pgpch.cpp"

	filter "system:macosx"
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
		"Pagoda/vendor/spdlog/include",
		"Pagoda/vendor/glfw/include",
		"Pagoda/vendor/eigen",
		"Pagoda/vendor/glad/glad/include",
		"Pagoda/vendor/lua/lua/include"
	}

	includedirs {
		table.unpack(incDirs)
	}

	libdirs {
		"bin/%{outputdir}/Pagoda",
		"Pagoda/vendor/lua/bin/%{outputdir}/lua",
		"Pagoda/vendor/glad/bin/%{outputdir}/glad",
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
			"pagoda",
			"spdlogd",
			"glfw3",
			"OpenGL32",
			"glad",
			"lua"
		}

	filter {"system:windows", "configurations:Release"}
	libdirs {
		"Pagoda/vendor/spdlog/build/Release",
		"Pagoda/vendor/glfw/src/Release"
	}

		links {
			"pagoda",
			"spdlog",
			"glfw3",
			"OpenGL32",
			"glad",
			"lua"
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
			"pagoda",
			"glfw3",
			"lua",
			"glad"
		}

	filter "system:linux"
		sysincludedirs {
			table.unpack(incDirs)
		}
		links {
			"pthread",
			"dl",
			"spdlog",
			"pagoda",
			"glfw3",
			"lua",
			"glad"
		}