cmake_minimum_required (VERSION 3.13...3.21)

project(ppg-core VERSION 1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB PPG_CORE_SRC
    "src/*.h"
    "src/*.cpp"
    "src/core/*.h"
    "src/core/*.cpp"
)

file(GLOB PPG_EXAMPLE_SRC
	"example/*.h"
	"example/*.cpp"
)

add_library(ppg-core ${PPG_CORE_SRC})


add_executable(ppg-core-example ${PPG_EXAMPLE_SRC})

target_link_libraries(ppg-core-example PUBLIC ppg-core)
target_include_directories(ppg-core-example PUBLIC "src/")