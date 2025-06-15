#!/bin/bash

# OpenCV Development Environment Setup Script
# This script sets up the build environment and provides common development commands

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
CMAKE_DIR="$SCRIPT_DIR/../cmake"

# Set up environment
export PATH="$CMAKE_DIR/bin:$BUILD_DIR:$PATH"

echo "OpenCV Development Environment"
echo "=============================="
echo "Build directory: $BUILD_DIR"
echo "Available tools: cmake, ninja"
echo ""

# Function to display available commands
show_help() {
    echo "Available commands:"
    echo "  configure  - Configure OpenCV build"
    echo "  build      - Build OpenCV (core modules)"
    echo "  build-all  - Build all OpenCV modules"
    echo "  test       - Run core tests"
    echo "  test-all   - Run all available tests"
    echo "  demo       - Run simple computer vision demo"
    echo "  clean      - Clean build directory"
    echo "  help       - Show this help"
}

# Configure OpenCV
configure_opencv() {
    echo "Configuring OpenCV..."
    cd "$BUILD_DIR"
    cmake -G "Ninja" \
        -DCMAKE_C_COMPILER=/usr/bin/gcc \
        -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_EXAMPLES=OFF \
        -DBUILD_DOCS=OFF \
        -DBUILD_PERF_TESTS=OFF \
        -DBUILD_TESTS=ON \
        ..
}

# Build core modules
build_opencv() {
    echo "Building OpenCV core modules..."
    cd "$BUILD_DIR"
    ninja opencv_core opencv_imgproc opencv_test_core
}

# Build all modules
build_all_opencv() {
    echo "Building all OpenCV modules..."
    cd "$BUILD_DIR"
    ninja -j4
}

# Run tests
run_tests() {
    echo "Running core tests..."
    cd "$BUILD_DIR"
    ./bin/opencv_test_core --gtest_color=yes
}

# Run all tests
run_all_tests() {
    echo "Running all available tests..."
    cd "$BUILD_DIR"
    for test_exe in bin/opencv_test_*; do
        if [[ -x "$test_exe" ]]; then
            echo "Running $(basename "$test_exe")..."
            "$test_exe" --gtest_color=yes
        fi
    done
}

# Clean build
clean_build() {
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"/*
}

# Run simple demo
run_demo() {
    echo "Running simple computer vision demo..."
    cd "$SCRIPT_DIR/examples"
    
    # Compile if needed
    if [[ ! -f simple_demo ]] || [[ simple_demo.cpp -nt simple_demo ]]; then
        echo "Compiling demo..."
        /usr/bin/g++ -std=c++11 -O2 simple_demo.cpp -o simple_demo
    fi
    
    # Run demo
    echo "Starting demo..."
    ./simple_demo
}

# Main command handler
case "${1:-help}" in
    configure)
        configure_opencv
        ;;
    build)
        build_opencv
        ;;
    build-all)
        build_all_opencv
        ;;
    test)
        run_tests
        ;;
    test-all)
        run_all_tests
        ;;
    clean)
        clean_build
        ;;
    demo)
        run_demo
        ;;
    help|*)
        show_help
        ;;
esac