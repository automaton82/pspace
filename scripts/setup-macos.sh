# PSpace Setup for macOS
# Run: brew install cmake sdl2 sdl2_image libpng openal-soft

echo "=== PSpace Development Environment Setup ==="
echo "Setting up dependencies for macOS..."

# Check if Homebrew is installed
if ! command -v brew &> /dev/null; then
    echo "Error: Homebrew is required but not installed."
    echo "Install it from: https://brew.sh"
    exit 1
fi

echo "Installing dependencies via Homebrew..."
brew install \
    cmake \
    sdl2 \
    sdl2_image \
    libpng \
    openal-soft

echo ""
echo "=== All dependencies installed successfully! ==="
echo "Run: ./scripts/build.sh to build the project"
