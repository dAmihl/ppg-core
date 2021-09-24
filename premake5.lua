workspace "PPG"
	language "C++"
	cppdialect "C++14"
	configurations { "Debug", "Release"}
	
	include "ppg-core.lua"	
	
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