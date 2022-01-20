workspace "Pagoda"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Pagoda"
	location "Pagoda"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pgpch.h"
	pchsource "Pagoda/src/Pagoda/pgpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/Pagoda",
		"%{prj.name}/vendor/spdlog/include",
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PG_PLATFORM_WINDOWS",
			"PG_ENABLE_ASSERTS",
			"PG_DEBUG",
			"SPDLOG_COMPILED_LIB"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/TestApp")
		}

	filter "configurations:Debug"
		defines "PG_DEBUG"
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
	
	libdirs
	{
		"bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Pagoda",
		"Pagoda/vendor/spdlog/build/%{cfg.buildcfg}"
	}

	links
	{
		"Pagoda.lib",
		"spdlogd.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"PG_PLATFORM_WINDOWS",
			"PG_ENABLE_ASSERTS",
			"PG_DEBUG",
			"SPDLOG_COMPILED_LIB"
		}

	filter "configurations:Debug"
		defines "CF_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "CF_RELEASE"
		buildoptions "/MD"

		optimize "On"
	filter "configurations:Dist"
		defines "CF_DIST"
		buildoptions "/MD"
		optimize "On"