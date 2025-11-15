# OpenGL 2D Black Hole Simulator

## Step by step version of this project can be found at:

https://www.hellocpp.dev/projects/opengl-blackhole-2d

This project creates a 2D black hole gravitational lensing simulation using OpenGL and the Schwarzschild metric. You'll visualize how light rays bend around a massive black hole using numerical integration.

## Quick Start (Recommended)

**Use the automated build scripts included with your download:**

### Linux/macOS
```bash
chmod +x run.sh
./run.sh
```

### Windows
```cmd
build.bat
```

**The scripts will:**
- Check for CMake and C++ compiler
- Automatically use vcpkg if `VCPKG_ROOT` is set
- Fall back to system libraries otherwise
- Build and run your project
- Provide helpful error messages if dependencies are missing

**If the script fails**, follow the manual installation instructions below to install dependencies, then run the script again.

---

## Building Requirements

- C++ Compiler supporting C++17 or newer
- [CMake](https://cmake.org/)
- [Vcpkg](https://vcpkg.io/en/) (recommended) or system package manager
- [Git](https://git-scm.com/)

## Dependencies

- **GLEW** (OpenGL Extension Wrangler Library)
- **GLFW** (Window and input management)
- **GLM** (OpenGL Mathematics library)

# Manual Build Instructions

If you prefer manual control or the automated scripts fail, follow these detailed instructions.

## Method 1: Using Vcpkg (Recommended for Windows)

**Step 1: Install vcpkg** (if you don't have it)

```bash
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Bootstrap vcpkg
.\bootstrap-vcpkg.bat              # Windows
./bootstrap-vcpkg.sh               # Linux/macOS
```

**Step 2: Navigate to your project directory**

```bash
cd /path/to/your/project
```

**Step 3: Install dependencies using vcpkg.json**

This project includes a `vcpkg.json` file that automatically specifies all dependencies.

```bash
vcpkg install
```

This will install: GLEW, GLFW3, and GLM.

**Step 4: Get the vcpkg CMake toolchain file path**

```bash
vcpkg integrate install
```

This outputs something like:
```
CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
```

Copy the path shown in your output (you'll need it in the next step).

**Step 5: Configure the project with CMake**

Replace `/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake` with the actual path from Step 4:

```bash
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
```

**Windows example:**
```cmd
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
```

**Step 6: Build the project**

```bash
cmake --build build
```

**Step 7: Run the program**

The executable will be in the `build` folder:

```bash
./build/blackhole                  # Linux/macOS
build\Debug\blackhole.exe          # Windows (Debug)
build\Release\blackhole.exe        # Windows (Release)
```

---

## Method 2: macOS (Homebrew)

Install dependencies via [Homebrew](https://brew.sh/):

```bash
# Install dependencies
brew install cmake glew glfw glm

# Build the project
cmake -B build -S .
cmake --build build

# Run the program
./build/blackhole
```

## Method 3: Debian/Ubuntu (System Packages)

Install dependencies via apt:

```bash
# Install dependencies
sudo apt update
sudo apt install build-essential cmake libglew-dev libglfw3-dev libglm-dev libgl1-mesa-dev

# Build the project
cmake -B build -S .
cmake --build build

# Run the program
./build/blackhole
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

### Report Issues

Found a bug or have suggestions? Please report issues on GitHub:

**https://github.com/helloCppOrg/OpenGL-2D-Blackhole-Simulator/issues**

When reporting issues, please include:
- Your operating system and version
- CMake version (`cmake --version`)
- Compiler version (`c++ --version` or `g++ --version`)
- Full error message or unexpected behavior
- Steps to reproduce the issue

Happy coding!
