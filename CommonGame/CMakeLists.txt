# CommonGame library CMakeLists.txt

# Collect all source files
file(GLOB_RECURSE COMMONGAME_SOURCES
    "*.cpp"
    "*.c"
)

# Collect all header files
file(GLOB_RECURSE COMMONGAME_HEADERS
    "*.h"
    "*.hpp"
)

# Remove any platform-specific files we don't need for cross-platform
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Remove .old files
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*\\.old\\.cpp$")

# Remove SubspaceStubs.cpp since we're using real implementations
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*SubspaceStubs\\.cpp$")

# Remove Subspace-specific files that shouldn't be in CommonGame
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*ChatBuffer\\.cpp$")

# Remove Windows-specific DirectInput/DirectAudio files
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*DInput.*\\.cpp$")
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*DAudio.*\\.cpp$")
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*WinUtil.*\\.cpp$")

# Add Linux DirectInput stubs
if(NOT WIN32)
    list(APPEND COMMONGAME_SOURCES 
        "${CMAKE_CURRENT_SOURCE_DIR}/DInputInterface_Linux.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/DInputKeyboard_Linux.cpp"
    )
endif()

# Remove Windows-specific threading files (need porting)
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*Thread.*\\.cpp$")
# Add Linux Thread stub for interim functionality
list(APPEND COMMONGAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Thread_Linux.cpp")
# Add Linux Thread stub for interim functionality
list(APPEND COMMONGAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Thread_Linux.cpp")
# Add Subspace color and sound globals 
list(APPEND COMMONGAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/SubspaceColors.cpp")
list(APPEND COMMONGAME_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/SubspaceSounds.cpp")
# TimeProfiler.cpp - include it for game linking
# list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*TimeProfiler.*\\.cpp$")
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*TimeStream.*\\.cpp$")

# Remove files with complex inheritance issues (temporary)
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*FileLog.*\\.cpp$")
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*WindowsConsole.*\\.cpp$")

# Remove OpenGL extension and Windows-specific graphics files
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*GLExtensions.*\\.cpp$")
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*GLPBuffer.*\\.cpp$")
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*GLObject\\.cpp$")

# Remove networking files (complex to port)
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*Socket.*\\.cpp$")
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*GameConnection.*\\.cpp$")

# Remove more Windows-specific files
list(FILTER COMMONGAME_SOURCES EXCLUDE REGEX ".*GameHud.*\\.cpp$")

# Create the CommonGame library
add_library(CommonGame STATIC ${COMMONGAME_SOURCES} ${COMMONGAME_HEADERS})

# Explicitly add SDL2 port files that might have been filtered out
target_sources(CommonGame PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/SDLGLObject.cpp
)

# Include directories for CommonGame
target_include_directories(CommonGame PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
    ${ALUT_INCLUDE_DIRS}
)

# Link libraries for CommonGame
target_link_libraries(CommonGame PUBLIC
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${PNG_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${ALUT_LIBRARIES}
    ${PLATFORM_LIBS}
)

# Compiler flags
target_compile_options(CommonGame PRIVATE ${SDL2_CFLAGS_OTHER})