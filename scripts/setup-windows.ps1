# PSpace Setup for Windows
# This script sets up dependencies for Windows development

Write-Host "=== PSpace Development Environment Setup ===" -ForegroundColor Green
Write-Host "Setting up dependencies for Windows..."

# Check if vcpkg is available
if (!(Get-Command vcpkg -ErrorAction SilentlyContinue)) {
    Write-Host "Installing vcpkg (C++ package manager)..." -ForegroundColor Yellow
    Write-Host "Please install vcpkg manually from: https://github.com/Microsoft/vcpkg"
    Write-Host "Then run: vcpkg integrate install"
    exit 1
}

Write-Host "Installing dependencies via vcpkg..."
vcpkg install sdl2:x64-windows
vcpkg install sdl2-image:x64-windows  
vcpkg install libpng:x64-windows
vcpkg install openal-soft:x64-windows

Write-Host ""
Write-Host "=== All dependencies installed successfully! ===" -ForegroundColor Green
Write-Host "Open the project in Visual Studio or VS Code with CMake extension"
