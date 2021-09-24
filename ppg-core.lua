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