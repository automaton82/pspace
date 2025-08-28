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

# Create the CommonGame library
add_library(CommonGame STATIC ${COMMONGAME_SOURCES} ${COMMONGAME_HEADERS})

# Include directories for CommonGame
target_include_directories(CommonGame PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${SDL2_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
)

# Link libraries for CommonGame
target_link_libraries(CommonGame PUBLIC
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${PNG_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${PLATFORM_LIBS}
)

# Compiler flags
target_compile_options(CommonGame PRIVATE ${SDL2_CFLAGS_OTHER})