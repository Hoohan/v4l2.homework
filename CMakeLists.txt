cmake_minimum_required(VERSION 3.14)

project(v4l2.homework)

# set C++ standard and extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set the output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)

# export compile commands to setup clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# using PkgConfig to find gtkmm-3.0
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

# public include directory
include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(test)