cmake_minimum_required(VERSION 3.16)

project(pspace VERSION 0.190 LANGUAGES CXX C)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
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
find_package(PkgConfig REQUIRED)
pkg_check_modules(ALUT REQUIRED freealut)

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
# Note: Subspace application excluded temporarily due to architectural issues
# add_subdirectory(Subspace/subspace)

# Add test program
add_executable(test_sdl2 test_sdl2.cpp)

target_include_directories(test_sdl2 PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
)

target_link_libraries(test_sdl2 PRIVATE
    CommonGame
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

target_compile_options(test_sdl2 PRIVATE ${SDL2_CFLAGS_OTHER})

# Add executable for input command system demo (working version)
add_executable(input_demo
    input_demo_main.cpp
    SDLInputBridge.cpp
    InputCommandGenerator.cpp
    Subspace/subspace/InputCommands.cpp
)

target_include_directories(input_demo PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CommonGame
    ${SDL2_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
)

target_link_libraries(input_demo PRIVATE
    CommonGame
    ${SDL2_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

target_compile_options(input_demo PRIVATE ${SDL2_CFLAGS_OTHER})

# Build new SDL2-based Subspace test application
add_executable(subspace_test subspace_main.cpp SubspaceMock.cpp SDLInputBridge.cpp InputCommandGenerator.cpp Subspace/subspace/InputCommands.cpp)

target_include_directories(subspace_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CommonGame
    ${CMAKE_CURRENT_SOURCE_DIR}/Subspace/subspace
    ${SDL2_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
)

target_link_libraries(subspace_test PRIVATE
    CommonGame
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${PNG_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${PLATFORM_LIBS}
)

target_compile_options(subspace_test PRIVATE ${SDL2_CFLAGS_OTHER})

# Build full Subspace game application with real game manager
# We need to include specific Subspace sources for the game manager
file(GLOB SUBSPACE_GAME_SOURCES
    "Subspace/subspace/*.cpp"
    "Subspace/subspace network library/*.cpp"
)

# Filter out .old files and duplicates
list(FILTER SUBSPACE_GAME_SOURCES EXCLUDE REGEX ".*\\.old\\.cpp$")
# Exclude network library main.cpp and duplicate files (prefer subspace/ versions)
list(FILTER SUBSPACE_GAME_SOURCES EXCLUDE REGEX "Subspace/subspace network library/main\\.cpp$")
list(FILTER SUBSPACE_GAME_SOURCES EXCLUDE REGEX "Subspace/subspace network library/SubspaceNetworkHandler\\.cpp$")
list(FILTER SUBSPACE_GAME_SOURCES EXCLUDE REGEX "Subspace/subspace network library/SubspaceReliableManager\\.cpp$")
# Also exclude the other main.cpp files, we'll use our own
list(FILTER SUBSPACE_GAME_SOURCES EXCLUDE REGEX "Subspace/subspace/main\\.cpp$")

add_executable(subspace_full subspace_full.cpp SDLInputBridge.cpp ${SUBSPACE_GAME_SOURCES})

target_include_directories(subspace_full PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CommonGame
    ${CMAKE_CURRENT_SOURCE_DIR}/Subspace/subspace
    "${CMAKE_CURRENT_SOURCE_DIR}/Subspace/subspace network library"
    "${CMAKE_CURRENT_SOURCE_DIR}/Subspace/subspace LVZ deconstructor"
    ${SDL2_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
)

# Link against both CommonGame and the actual Subspace game code
target_link_libraries(subspace_full PRIVATE
    CommonGame
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${PNG_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${PLATFORM_LIBS}
)

target_compile_options(subspace_full PRIVATE ${SDL2_CFLAGS_OTHER})

# VS Code integration files
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.vscode")
    message(STATUS "VS Code configuration found")
endif()