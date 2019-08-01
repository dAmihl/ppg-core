cmake_minimum_required (VERSION 2.6)
SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
SET (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
project (ppg-core)

add_subdirectory(src)
add_subdirectory(test)
enable_testing ()
add_test(Name GeneralTest COMMAND test)