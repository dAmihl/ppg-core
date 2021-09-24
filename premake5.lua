workspace "PPG"
	language "C++"
	cppdialect "C++14"
	configurations { "Debug", "Release"}
	
project "PPG"
	kind "StaticLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	files {"src/**.h", "src/**.cpp", "src/core/**.h", "src/core/**.cpp"}
	
	includedirs {"vendor/includes"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"
		
	filter "configurations:Release"
		defines {"NDEBUG"}
		symbols "On"
		
	
---------------- Testing
project ("PPG-tests")
    targetdir "bin"
    kind "ConsoleApp"
    files {"test/**.cpp"}
    includedirs {"vendor/includes", "src"}
    links {"PPG"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"
		
	filter "configurations:Release"
		defines {"NDEBUG"}
		symbols "On"
		
		
--------------- Example
project ("PPG-example")
	location "example"
	targetdir "bin"
	kind "ConsoleApp"
	files {"example/*.cpp"}
	includedirs {"src"}
	links {"PPG"}
	
	filter "configurations:Debug"
		defines {"DEBUG"}
		symbols "On"
		
	filter "configurations:Release"
		defines {"NDEBUG"}
		symbols "On"