project(unit_tests)
add_subdirectory(lib)
include_directories($(gtest_SOURCE_DIR)/include $(gtest_SOURCE_DIR))

add_executable(unit_tests_run Cumulative_mat.cpp)
target_link_libraries(unit_tests_run gtest gtest_main)

