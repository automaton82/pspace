#!/bin/bash
set -e

echo "=== Building PSpace ==="

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
fi

# Configure with CMake
echo "Configuring with CMake..."
cd build
cmake ..

# Build with parallel jobs
echo "Building with make..."
make -j$(nproc)

echo ""
echo "=== Build completed successfully! ==="
echo ""
echo "To run the SDL2 test:"
echo "  ./test_sdl2"
echo ""
echo "To run from project root:"
echo "  cd build && ./test_sdl2"
