# PSpace Development Guide

## VS Code Setup

### Automatic Setup
1. Open the project in VS Code
2. You'll be prompted to install recommended extensions - click "Install"
3. The extensions will be configured automatically

### Manual Extension Installation
If you need to install extensions manually:
- **C/C++** (`ms-vscode.cpptools`) - IntelliSense, debugging, code browsing
- **CMake Tools** (`ms-vscode.cmake-tools`) - CMake integration
- **CMake** (`twxs.cmake`) - CMake language support

## Building in VS Code

### Quick Build
- Press `Ctrl+Shift+B` (Linux/Windows) or `Cmd+Shift+B` (macOS)
- This runs the default build task

### CMake Integration
- Press `Ctrl+Shift+P` → "CMake: Configure" to configure the project
- Press `Ctrl+Shift+P` → "CMake: Build" to build
- The bottom status bar shows CMake status and build progress

## Running and Debugging

### Run the SDL2 Test
- Press `F5` to start debugging
- Or press `Ctrl+F5` to run without debugging
- Or use `Ctrl+Shift+P` → "Debug: Start Debugging"

### Available Launch Configurations
1. **Debug SDL2 Test** - Normal debugging with breakpoints
2. **Debug CommonGame Library** - Debug with immediate breakpoint at entry

### Terminal Commands
You can also use the integrated terminal:
```bash
# Build
./scripts/build.sh

# Run
cd build && ./test_sdl2
```

## Code Navigation

### IntelliSense Features
- **Auto-completion**: Type and see suggestions
- **Go to Definition**: `F12` or `Ctrl+Click`
- **Find All References**: `Shift+F12`
- **Symbol Search**: `Ctrl+T` to search for functions/classes

### Project Structure
- `CommonGame/` - Core engine library
- `Subspace/` - Game application (needs refactoring)
- `test_sdl2.cpp` - SDL2 integration test
- `.vscode/` - VS Code configuration

## Tasks and Scripts

### Available Tasks (`Ctrl+Shift+P` → "Tasks: Run Task")
- **build** - Build the entire project
- **clean** - Clean build artifacts

### Command Line Scripts
- `./scripts/setup-linux.sh` - Install all dependencies
- `./scripts/build.sh` - Build the project
- `./scripts/setup-macos.sh` - macOS setup
- `./scripts/setup-windows.ps1` - Windows setup

## Troubleshooting

### Common Issues

1. **"CMake not found"**
   - Run `./scripts/setup-linux.sh` to install dependencies
   - Or manually install: `sudo apt install cmake`

2. **"SDL2 headers not found"**
   - Run setup script or manually install: `sudo apt install libsdl2-dev`

3. **Build fails with missing libraries**
   - Ensure all dependencies are installed via setup script
   - Check CMake output for specific missing libraries

4. **IntelliSense not working**
   - Press `Ctrl+Shift+P` → "C/C++: Reset IntelliSense Database"
   - Ensure C/C++ extension is installed and enabled

### Clean Build
If you encounter build issues:
```bash
rm -rf build/
./scripts/build.sh
```

### Debugging Tips
- Set breakpoints by clicking in the left margin
- Use the Debug Console to evaluate expressions
- Watch variables in the Variables panel
- Step through code with F10 (step over) and F11 (step into)

## Next Development Steps

### Currently Working
- ✅ CommonGame library builds and runs
- ✅ SDL2/OpenGL integration functional
- ✅ Cross-platform build system
- ✅ VS Code development environment

### Needs Work
1. **Subspace Application Refactoring**
   - Input system needs SDL2 integration
   - Command system needs updating
   - Network layer compatibility

2. **Future Targets**
   - WebAssembly compilation via Emscripten
   - Mobile platform support
   - Modern OpenGL shader pipeline
