cmake_minimum_required(VERSION 3.16)

project(pspace VERSION 0.190 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)

find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(OpenAL REQUIRED)

# Platform-specific settings
if(WIN32)
    set(PLATFORM_LIBS ws2_32)
else()
    set(PLATFORM_LIBS ${CMAKE_DL_LIBS})
endif()

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OpenGL_INCLUDE_DIRS})

# Add subdirectories
add_subdirectory(CommonGame)
add_subdirectory(Subspace/subspace)

# VS Code integration files
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.vscode")
    message(STATUS "VS Code configuration found")
endif()