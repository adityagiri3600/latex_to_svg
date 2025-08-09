# latex_to_svg

A LaTeX to SVG converter written in C.

## Quick Start

### Download Pre-built Binaries
- **Linux**: Download `l2s-linux-x64.tar.gz` from [Releases](https://github.com/adityagiri3600/latex_to_svg/releases)
- **Windows**: Download `l2s-windows-x64.zip` from [Releases](https://github.com/adityagiri3600/latex_to_svg/releases)

### Quick Test
```bash
# Linux/macOS
echo "x_1 + \frac{x^2+5}{1}" | ./l2s > equation.svg

# Windows
echo x_1 + \frac{x^2+5}{1} | l2s.exe > equation.svg
```

## Building from Source

### Prerequisites

#### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install cmake gcc flex bison
```

#### Linux (Fedora/RHEL)
```bash
sudo dnf install cmake gcc flex bison
```

#### macOS
```bash
brew install cmake gcc flex bison
```

#### Windows
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

### Building

```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

## Running

After building, the executable will be located at:
- Linux/macOS: `build/l2s`
- Windows: `build/Release/l2s.exe` (Visual Studio) or `build/l2s.exe` (MinGW)