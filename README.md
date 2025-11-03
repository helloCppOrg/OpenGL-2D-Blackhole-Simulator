# OpenGL 2D Black Hole Simulator

This project creates a 2D black hole gravitational lensing simulation using OpenGL and the Schwarzschild metric. You'll visualize how light rays bend around a massive black hole using numerical integration.

## Building Requirements

- C++ Compiler supporting C++17 or newer
- [CMake](https://cmake.org/)
- [Vcpkg](https://vcpkg.io/en/) (recommended) or system package manager
- [Git](https://git-scm.com/)

## Dependencies

- **GLEW** (OpenGL Extension Wrangler Library)
- **GLFW** (Window and input management)
- **GLM** (OpenGL Mathematics library)

# Build Instructions

See below for alternatives.

### 1. Install Dependencies with Vcpkg

If you already have vcpkg installed, run:
```bash
vcpkg install glew glfw3 glm
```

If you don't have vcpkg:
```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Bootstrap (Windows)
.\bootstrap-vcpkg.bat

# Bootstrap (macOS/Linux)
./bootstrap-vcpkg.sh

# Install dependencies
./vcpkg install glew glfw3 glm

# Print out CMake Toolchain Path
./vcpkg integrate install
```

If you cannot get the dependencies to install this jump to `Alternative: Debian/Ubuntu apt` and come back here.

### 2. Get Vcpkg CMake Toolchain Path

If you installed vcpkg above get the path copy the path from there for the next step and skip this.

```bash
vcpkg integrate install
```

This will output something like:
```
CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

Copy the path for the next step.

### 3. Build the Project

```bash
# Configure with CMake (use your vcpkg path from step 2)
# will look like:
# cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake -B build -S . [YOUR DCMAKE_TOOLCHAIN_FILE]

# Build
cmake --build build

# Run (executable will be in build folder)
./build/blackhole
```

## Alternative: Debian/Ubuntu apt

If you don't want to use vcpkg, install these packages and run the normal CMake steps:

```bash
sudo apt update
sudo apt install build-essential cmake \
    libglew-dev libglfw3-dev libglm-dev libgl1-mesa-dev
```

Then build without the vcpkg toolchain file:
```bash
cmake -B build -S .
cmake --build build
```

## macOS Users (alternative)

[Homebrew](https://brew.sh/) is an alternative option:

```bash
# Install dependencies
brew install cmake glew glfw glm

# Build
cmake -B build -S .
cmake --build build
./build/blackhole2d
```

## What You'll Build

This 2D simulator demonstrates:
- Light ray bending around a black hole
- Schwarzschild metric implementation
- Runge-Kutta 4th order (RK4) numerical integration
- Real-time visualization of gravitational lensing

## Learning Resources

- [Learn OpenGL](https://learnopengl.com/) - Comprehensive OpenGL tutorial
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [Schwarzschild Metric](https://en.wikipedia.org/wiki/Schwarzschild_metric) - Physics background
- [Gravitational Lensing](https://en.wikipedia.org/wiki/Gravitational_lens) - Physics background

## Support

If you encounter compilation errors:

1. **Verify all dependencies are installed** using the commands in your platform's section
2. **Check CMake output** for specific missing libraries
3. **Update graphics drivers** to the latest version
4. **Ensure C++17 support** - use GCC 7+, Clang 5+, or MSVC 2017+

Happy coding!
