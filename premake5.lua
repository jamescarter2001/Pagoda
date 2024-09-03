workspace "Pagoda"

	configurations {
		"Debug",
		"Release",
	}

	platforms {
		"Win32",
		"ARM64",
		"x64",
		"Linux",
		"MacOS",
	}
	
	filter { "platforms:ARM64" }
		system "windows"
		architecture "arm64"
		defines "PG_PLATFORM_WINDOWS"
		defines "PG_ARM64"
		
	filter { "platforms:Win32" }
		system "windows"
		architecture "x86"
		defines "PG_PLATFORM_WINDOWS"

	filter { "platforms:x64" }
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
		defines "PG_ARM64"
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
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/imgui/*.h",
		"%{prj.name}/vendor/imgui/*.cpp",
		
		"%{prj.name}/vendor/imgui/backends/imgui_impl_dx12.h",
		"%{prj.name}/vendor/imgui/backends/imgui_impl_dx12.cpp",
		
		"%{prj.name}/vendor/imgui/backends/imgui_impl_win32.h",
		"%{prj.name}/vendor/imgui/backends/imgui_impl_win32.cpp"
	}

	defines {
		"SPDLOG_COMPILED_LIB",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
	}

	incDirs = {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/vendor/DirectX-Headers/include",
		"%{prj.name}/vendor/glm",
		"%{prj.name}/vendor/imgui"
	}

	includedirs {
		table.unpack(incDirs)
	}

	pchheader "pgpch.h"
	pchsource "Pagoda/src/pgpch.cpp"

	filter "system:macosx"
		externalincludedirs { --[[  sysincludedirs --]]
			table.unpack(incDirs)
		}

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
		
	filter "system:windows"
		files {
		"%{prj.name}/res/**.hlsl"
		}
		
	filter "files:**.hlsl"
		shadertype "Vertex"
		shaderentry "vs_main"
		shadermodel "5.0"
		
project "DatabaseTestApp"
	location "DatabaseTestApp"
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
		"SPDLOG_COMPILED_LIB",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
	}

	includedirs {
		"Pagoda/src",
		"Pagoda/vendor/spdlog/include",
		"Pagoda/vendor/DirectX-Headers/include",
		"Pagoda/vendor/glm"
	}

	libdirs {
		"bin/%{outputdir}/Pagoda",
		"Pagoda/vendor/spdlog/build"
	}
	
	--[[postbuildcommands {
		"{COPY} ../Pagoda/res/mirage/platform/d3d11/shader ../bin/%{outputdir}/%{prj.name}"
	}]]--

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
	 externalincludedirs {
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
			"d3d11",
			"d3d12",
			"dxgi",
			"d3dcompiler"
		}

	filter {"system:windows", "configurations:Release"}
	libdirs {
		"Pagoda/vendor/spdlog/build/Release",
		"Pagoda/vendor/glfw/src/Release"
	}

		links {
			"pagoda",
			"spdlog",
			"d3d11",
			"d3d12",
			"dxgi",
			"d3dcompiler"
		}
		
	filter "system:macosx"
		links {
			"CoreFoundation.framework", 
			"Cocoa.framework",
	 		"IOKit.framework"
		}
		links {
			"spdlog",
			"pagoda"
		}

	filter "system:linux"
		externalincludedirs {
			table.unpack(incDirs)
		}
		links {
			"pthread",
			"dl",
			"spdlog",
			"pagoda"
		}

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
		"SPDLOG_COMPILED_LIB",
		"_SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS"
	}

	includedirs {
		"Pagoda/src",
		"Pagoda/vendor/spdlog/include",
		"Pagoda/vendor/DirectX-Headers/include",
		"Pagoda/vendor/glm",
		"Pagoda/vendor/imgui"
	}

	libdirs {
		"bin/%{outputdir}/Pagoda",
		"Pagoda/vendor/spdlog/build"
	}
	
	--[[postbuildcommands {
		"{COPY} ../Pagoda/res/mirage/platform/d3d11/shader ../bin/%{outputdir}/%{prj.name}"
	}]]--

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
	 externalincludedirs {
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
			"d3d11",
			"d3d12",
			"dxgi",
			"d3dcompiler"
		}

	filter {"system:windows", "configurations:Release"}
	libdirs {
		"Pagoda/vendor/spdlog/build/Release",
		"Pagoda/vendor/glfw/src/Release"
	}

		links {
			"pagoda",
			"spdlog",
			"d3d11",
			"d3d12",
			"dxgi",
			"d3dcompiler"
		}
		
	filter "system:macosx"
		links {
			"CoreFoundation.framework", 
			"Cocoa.framework",
	 		"IOKit.framework"
		}
		links {
			"spdlog",
			"pagoda"
		}

	filter "system:linux"
		externalincludedirs {
			table.unpack(incDirs)
		}
		links {
			"pthread",
			"dl",
			"spdlog",
			"pagoda"
		}