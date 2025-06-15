# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

OpenCV (Open Source Computer Vision Library) is a large-scale computer vision and machine learning library written primarily in C++. The codebase follows a modular architecture with each major functionality organized into separate modules.

## Build System

OpenCV uses CMake as its primary build system with out-of-source builds required.

### Development Environment

This repository has a pre-configured development environment. Use the provided build script:

```bash
# Set up and use the development environment
./build_env.sh help          # Show available commands
./build_env.sh configure     # Configure build (one-time setup)
./build_env.sh build         # Build core modules
./build_env.sh build-all     # Build all modules
./build_env.sh test          # Run core tests
./build_env.sh test-all      # Run all tests
./build_env.sh clean         # Clean build directory
```

The environment includes:
- CMake 3.29.3 (downloaded and configured)
- Ninja build system (fast parallel builds)
- GCC 13.3.0 compiler
- Pre-configured build settings for development

### Manual Build Commands

```bash
# Create build directory and configure
mkdir build && cd build
export PATH=$PWD/../cmake/bin:$PATH
cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release ..

# Build using Ninja (faster than make)
ninja -j4
```

### Platform-Specific Builds

- **JavaScript/WebAssembly**: Use `platforms/js/build_js.py`
- **Android**: Use `platforms/android/build_sdk.py` 
- **iOS**: Use `platforms/ios/build_framework.py`
- **macOS**: Use `platforms/osx/build_framework.py`

## Testing

OpenCV has comprehensive test suites for accuracy and performance testing.

### Test Commands

```bash
# Using the build environment script (recommended)
./build_env.sh test         # Run core tests
./build_env.sh test-all     # Run all available tests

# Manual test execution
cd build
./bin/opencv_test_core --gtest_color=yes
./bin/opencv_test_core --gtest_filter="Core_Array.*"

# Run all tests via CTest (if available)
ctest

# Run specific test patterns
ctest -R opencv_test_core
ctest -R opencv_perf_
```

### Test Types

- `opencv_test_*` - Accuracy/unit tests
- `opencv_perf_*` - Performance tests

Each module defines tests using:
- `ocv_add_accuracy_tests()` - Unit/accuracy tests
- `ocv_add_perf_tests()` - Performance benchmarks

## Code Quality

### Python Linting

```bash
# Run pylint on Python code
make check_pylint
# or 
cmake --build . --target check_pylint
```

Configuration in `platforms/scripts/pylintrc`

### Documentation Generation

```bash
# Generate all documentation
make doxygen

# Specific documentation targets
make doxygen_cpp      # C++ API docs
make doxygen_python   # Python bindings docs
```

## Module Architecture

OpenCV is organized into core modules under `modules/`:

- **core** - Basic data structures (Mat, memory management, math operations)
- **imgproc** - Image processing algorithms (filtering, transforms, morphology)
- **imgcodecs** - Image I/O (JPEG, PNG, TIFF, etc.)
- **videoio** - Video capture/writing
- **highgui** - GUI operations (windows, user interaction)
- **calib3d** - Camera calibration and 3D reconstruction
- **features2d** - Feature detection and description
- **objdetect** - Object detection (Haar cascades, HOG)
- **dnn** - Deep neural network inference
- **ml** - Machine learning algorithms
- **photo** - Computational photography
- **stitching** - Image stitching
- **video** - Video analysis (tracking, optical flow)
- **gapi** - Graph API for pipeline optimization

Each module follows a standard structure:
- `include/` - Public headers
- `src/` - Implementation files
- `test/` - Accuracy tests
- `perf/` - Performance tests
- `CMakeLists.txt` - Build configuration

## Key Build Files

- Root `CMakeLists.txt` - Main build configuration
- `cmake/OpenCVModule.cmake` - Module build system
- `cmake/OpenCVUtils.cmake` - Utility functions
- Individual module `CMakeLists.txt` files

## Coding Conventions

- Follow the [OpenCV Coding Style Guide](https://github.com/opencv/opencv/wiki/Coding_Style_Guide)
- Use existing patterns when adding new functionality
- Each module typically has a `precomp.hpp` for precompiled headers
- Implementation files often have corresponding `.dispatch.cpp` and `.simd.hpp` for SIMD optimizations

## Testing Individual Modules

When working on a specific module, build and test just that module:

```bash
# Build specific module (example for core)
cmake --build . --target opencv_core

# Test specific module
ctest -R opencv_test_core
ctest -R opencv_perf_core
```

## Examples and Use Cases

The `examples/` directory contains practical OpenCV demonstrations:

### Quick Start - Simple Demo (Works Immediately)

```bash
cd examples
g++ -std=c++11 -O2 simple_demo.cpp -o simple_demo
./simple_demo
```

This standalone demo demonstrates core computer vision concepts without requiring the full OpenCV build.

### Available Examples

1. **Simple Demo** (`simple_demo.cpp`) - ✅ Ready to run
   - Matrix operations and basic CV concepts
   - No external dependencies required

2. **Image Operations** (`basic_cv/image_operations.cpp`) - 🔄 Requires full build
   - Loading, processing, and saving images
   - Color space conversions, filtering, enhancement

3. **Face Detection** (`face_detection/face_detector.cpp`) - 🔄 Requires full build + data
   - Real-time face and eye detection using Haar cascades
   - Performance measurement and result visualization

4. **Advanced CV** (`image_processing/advanced_cv.cpp`) - 🔄 Requires full build
   - Contour analysis, corner detection, morphological operations
   - Template matching and histogram analysis

### Running Examples

```bash
# Start with the simple demo
cd examples
./simple_demo

# For full examples (after building OpenCV)
./build_env.sh build-all
cd examples
# Compile and run specific examples as documented in examples/README.md
```

See `examples/README.md` for detailed instructions and learning path.