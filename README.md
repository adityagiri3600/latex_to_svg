# latex_to_svg

A LaTeX to SVG converter written in C.

## Prerequisites

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install cmake gcc flex bison
```

### Linux (Fedora/RHEL)
```bash
sudo dnf install cmake gcc flex bison
```

### macOS
```bash
brew install cmake gcc flex bison
```

### Windows
- Install Visual Studio with C++ support, or
- Install MSYS2 and run:
```bash
pacman -S cmake gcc flex bison
```
- Or install chocolatey and run:

```bash
choco install cmake gcc winflexbison3
```
Make sure to add the installation directory to your PATH.

## Building

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## Running

After building, the executable will be located at:
- Linux/macOS: `build/l2s`
- Windows: `build/Release/l2s.exe` (Visual Studio) or `build/l2s.exe` (MinGW)

## Usage

```bash
echo "x^2 + y" | ./build/l2s > output.svg
```