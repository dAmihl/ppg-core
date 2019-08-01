include_directories (${PROJECT_SOURCE_DIR}/includes ${PROJECT_SOURCE_DIR}/src)
add_executable (GeneralTest GeneralTest.cpp)
target_link_libraries (GeneralTest
                       ppg-core
                       )