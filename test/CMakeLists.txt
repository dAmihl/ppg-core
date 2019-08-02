include_directories (${PROJECT_SOURCE_DIR}/includes ${PROJECT_SOURCE_DIR}/src)
add_executable (PPGTests GeneralTest.cpp GeneratorUnitTests.cpp)
target_link_libraries (PPGTests
                       ppg-core
                       )