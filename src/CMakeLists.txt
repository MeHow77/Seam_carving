
cmake_minimum_required(VERSION 3.12)

project(seam-carving)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
  add_compile_options(/WX)
else()
  add_compile_options()
endif()

find_package(OpenCV REQUIRED)

add_library(sc
  seam_carving.cpp)

target_include_directories(sc SYSTEM
  PUBLIC ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(sc
  ${OpenCV_LIBS}
)

add_executable(seam-carving
  main.cpp
)

target_link_libraries(seam-carving
  sc
)

add_subdirectory(unit_tests)

