#!/bin/bash
set -e

echo "=== PSpace Development Environment Setup ==="
echo "Setting up dependencies for Linux/Ubuntu..."

# Update package list
echo "Updating package list..."
sudo apt update

# Essential build tools
echo "Installing build tools..."
sudo apt install -y \
    cmake \
    build-essential \
    git \
    pkg-config

# SDL2 and graphics libraries
echo "Installing SDL2 and graphics libraries..."
sudo apt install -y \
    libsdl2-dev \
    libsdl2-image-dev \
    libgl1-mesa-dev \
    freeglut3-dev

# Image processing
echo "Installing image processing libraries..."
sudo apt install -y \
    libpng-dev \
    zlib1g-dev

# Audio libraries
echo "Installing audio libraries..."
sudo apt install -y \
    libopenal-dev \
    libalut-dev

echo ""
echo "=== All dependencies installed successfully! ==="
echo ""
echo "Next steps:"
echo "1. Open this project in VS Code"
echo "2. Install recommended extensions (you'll be prompted)"
echo "3. Press Ctrl+Shift+B to build"
echo "4. Run: cd build && ./test_sdl2"
echo ""
echo "Or run: ./scripts/build.sh"
