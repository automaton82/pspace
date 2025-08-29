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

# Add subdirectories for the original 4-project structure
add_subdirectory(CommonGame)

# Add Subspace Network Library as a static library
file(GLOB NETWORK_LIB_SOURCES
    "Subspace/subspace network library/*.cpp"
)
# Exclude main.cpp from network library
list(FILTER NETWORK_LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(SubspaceNetworkLibrary STATIC ${NETWORK_LIB_SOURCES})

target_include_directories(SubspaceNetworkLibrary PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CommonGame
    "${CMAKE_CURRENT_SOURCE_DIR}/Subspace/subspace network library"
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(SubspaceNetworkLibrary PUBLIC
    CommonGame
    ${SDL2_LIBRARIES}
    ${PLATFORM_LIBS}
)

# Add Subspace LVZ Deconstructor as a static library
file(GLOB LVZ_SOURCES
    "Subspace/subspace LVZ deconstructor/*.cpp"
)
# Exclude main.cpp from LVZ library
list(FILTER LVZ_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_library(SubspaceLVZ STATIC ${LVZ_SOURCES})

target_include_directories(SubspaceLVZ PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/CommonGame
    "${CMAKE_CURRENT_SOURCE_DIR}/Subspace/subspace LVZ deconstructor"
)

target_link_libraries(SubspaceLVZ PUBLIC
    CommonGame
)

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

# Build full Subspace game application (main executable)
# Get only the main Subspace game sources (not network library or LVZ)
file(GLOB SUBSPACE_GAME_SOURCES
    "Subspace/subspace/*.cpp"
)

# Filter out .old files and main.cpp (we'll use our own)
list(FILTER SUBSPACE_GAME_SOURCES EXCLUDE REGEX ".*\\.old\\.cpp$")
list(FILTER SUBSPACE_GAME_SOURCES EXCLUDE REGEX ".*main\\.cpp$")
list(FILTER SUBSPACE_GAME_SOURCES EXCLUDE REGEX ".*SDLMain\\.cpp$")
list(FILTER SUBSPACE_GAME_SOURCES EXCLUDE REGEX ".*main-game\\.cpp$")  # Has duplicate globals

add_executable(subspace_full 
    subspace_full.cpp 
    SDLInputBridge.cpp 
    CommonGame/Socket_Linux_Stub.cpp
    CommonGame/FileLogger_Linux_Stub.cpp
    CommonGame/GameHud_Linux_Stub.cpp
    CommonGame/TimeStream_Linux_Stub.cpp
    ${SUBSPACE_GAME_SOURCES}
)

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

# Link against all the static libraries we built
target_link_libraries(subspace_full PRIVATE
    CommonGame
    SubspaceNetworkLibrary
    SubspaceLVZ
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