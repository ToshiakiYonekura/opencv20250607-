#!/bin/bash

# Compilation script for OpenCV examples
# This script compiles and runs OpenCV examples using the local build

echo "OpenCV Example Compilation Script"
echo "================================="

# Set up environment
BUILD_DIR="../build"
EXAMPLE_NAME="real_opencv_example"

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo "Error: Build directory not found at $BUILD_DIR"
    echo "Please build OpenCV first using: ./build_env.sh build"
    exit 1
fi

# Source the environment
if [ -f "$BUILD_DIR/setup_vars.sh" ]; then
    echo "Sourcing OpenCV environment..."
    source "$BUILD_DIR/setup_vars.sh"
else
    echo "Warning: setup_vars.sh not found, using manual paths"
fi

# Set library and include paths
OPENCV_LIBS="-L$BUILD_DIR/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui -lopencv_features2d -lopencv_calib3d -lopencv_video -lopencv_videoio -lopencv_objdetect -lopencv_photo -lopencv_ml -lopencv_dnn -lopencv_flann -lopencv_stitching -lopencv_gapi"
OPENCV_INCLUDES="-I../include -I$BUILD_DIR -I../modules/core/include -I../modules/imgproc/include -I../modules/imgcodecs/include -I../modules/highgui/include -I../modules/features2d/include -I../modules/calib3d/include -I../modules/video/include -I../modules/videoio/include -I../modules/objdetect/include -I../modules/photo/include -I../modules/ml/include -I../modules/dnn/include -I../modules/flann/include -I../modules/stitching/include -I../modules/gapi/include"

# Compile the example
echo "Compiling $EXAMPLE_NAME.cpp..."
g++ -std=c++11 -O2 \
    $OPENCV_INCLUDES \
    "$EXAMPLE_NAME.cpp" \
    $OPENCV_LIBS \
    -o "$EXAMPLE_NAME" \
    -Wl,-rpath,"$BUILD_DIR/lib"

if [ $? -eq 0 ]; then
    echo "✓ Compilation successful!"
    echo ""
    
    # Run the example
    echo "Running $EXAMPLE_NAME..."
    echo "========================"
    ./"$EXAMPLE_NAME"
    
    echo ""
    echo "✓ Example completed successfully!"
    
    # List generated files
    if ls *.png 1> /dev/null 2>&1; then
        echo ""
        echo "Generated image files:"
        ls -la *.png
    fi
else
    echo "✗ Compilation failed!"
    echo ""
    echo "Troubleshooting tips:"
    echo "1. Make sure OpenCV is built: ./build_env.sh build"
    echo "2. Check that all required libraries are present in $BUILD_DIR/lib/"
    echo "3. Verify headers are available in ../include/opencv2/"
    exit 1
fi