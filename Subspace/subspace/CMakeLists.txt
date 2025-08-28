# Subspace executable CMakeLists.txt

# Collect all source files
file(GLOB_RECURSE SUBSPACE_SOURCES
    "*.cpp"
    "*.c"
)

# Collect all header files
file(GLOB_RECURSE SUBSPACE_HEADERS
    "*.h"
    "*.hpp"
)

# Create the Subspace executable
add_executable(subspace ${SUBSPACE_SOURCES} ${SUBSPACE_HEADERS})

# Include directories for Subspace
target_include_directories(subspace PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../CommonGame
    ${SDL2_INCLUDE_DIRS}
    ${OpenGL_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIR}
)

# Link libraries for Subspace
target_link_libraries(subspace PRIVATE
    CommonGame
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${PNG_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${OPENAL_LIBRARY}
    ${PLATFORM_LIBS}
)

# Compiler flags
target_compile_options(subspace PRIVATE ${SDL2_CFLAGS_OTHER})

# Set output directory
set_target_properties(subspace PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)