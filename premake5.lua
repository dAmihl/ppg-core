workspace "PPG"
	language "C++"
	cppdialect "C++14"
	configurations { "Debug", "Release"}
	
project "PPG"
	kind "SharedLib"
	language "C++"
	targetdir "bin/%{cfg.buildcfg}"
	
	files {"src/**.h", "src/**.cpp"}
	
	includedirs {"includes"}
	
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
    includedirs {"includes", "src"}
    links {"PPG"}