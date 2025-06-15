# OpenCV Examples

This directory contains practical examples demonstrating OpenCV functionality and computer vision concepts.

## Quick Start

### Simple Demonstration (Working Now)

Run the conceptual demo that works immediately:

```bash
cd examples
g++ -std=c++11 -O2 simple_demo.cpp -o simple_demo
./simple_demo
```

This demonstrates core computer vision concepts without requiring the full OpenCV build.

## Available Examples

### 1. Simple Demo (`simple_demo.cpp`)
**Status: ✅ Working Now**

A standalone demonstration of computer vision concepts:
- Matrix operations and manipulation
- Basic image processing (blur, edge detection)
- Template matching simulation
- Corner detection concepts
- Statistical analysis

**Usage:**
```bash
g++ -std=c++11 -O2 simple_demo.cpp -o simple_demo
./simple_demo
```

### 2. Basic Image Operations (`basic_cv/image_operations.cpp`)
**Status: 🔄 Requires Full OpenCV Build**

Comprehensive image processing demonstration:
- Loading and saving images
- Color space conversions (BGR, HSV, Grayscale)
- Image filtering (Gaussian blur, edge detection)
- Histogram equalization
- Creating image mosaics

**Features:**
- Works with any image file or generates test images
- Creates multiple processed versions
- Saves results as image files

### 3. Face Detection (`face_detection/face_detector.cpp`)
**Status: 🔄 Requires Full OpenCV Build + Cascade Files**

Real-time face and eye detection:
- Haar cascade classifier usage
- Performance measurement
- Face and eye detection
- Drawing detection results
- Works with images or creates test patterns

**Requirements:**
- Haar cascade XML files (in `data/haarcascades/`)
- Full OpenCV build with objdetect module

### 4. Advanced Computer Vision (`image_processing/advanced_cv.cpp`)
**Status: 🔄 Requires Full OpenCV Build**

Advanced CV operations:
- Contour detection and analysis
- Harris corner detection
- Morphological operations (erosion, dilation, opening, closing)
- Template matching
- Color histogram analysis

## Building Examples

### Method 1: Using Build Environment (Recommended)

```bash
# From project root
./build_env.sh build        # Ensure OpenCV is built
cd examples
# Then compile individual examples as needed
```

### Method 2: Manual Compilation

For the simple demo (works immediately):
```bash
g++ -std=c++11 -O2 simple_demo.cpp -o simple_demo
```

For full OpenCV examples (requires complete build):
```bash
# Example compilation (when OpenCV build is complete)
g++ -I../build/include -std=c++11 \
    basic_cv/image_operations.cpp \
    -L../build/lib -lopencv_core -lopencv_imgproc -lopencv_imgcodecs -lopencv_highgui \
    -o image_operations
```

### Method 3: Using CMake (Future)

```bash
mkdir build && cd build
cmake -DOpenCV_DIR=../../build ..
ninja
```

## Example Outputs

### Simple Demo Output
```
=== Basic Matrix Operations ===
Matrix operations demonstration...

=== Image Processing Demo ===
Creating gradient image and applying filters...

=== Computer Vision Concepts ===
Template matching and feature detection simulation...
```

### Image Operations Output
```
OpenCV Basic Image Operations Demo
- output_original.jpg (input image)
- output_mosaic.jpg (6 different processing results)
```

### Face Detection Output
```
Face Detection Demo
- Faces detected: 2
- Eyes detected: 4
- Results saved to output_face_detection.jpg
```

## Learning Path

1. **Start with `simple_demo.cpp`** - Understand core concepts
2. **Build OpenCV** - Use `./build_env.sh build`
3. **Try `image_operations.cpp`** - Learn real image processing
4. **Experiment with `advanced_cv.cpp`** - Explore advanced algorithms
5. **Setup face detection** - Download cascade files and test recognition

## Computer Vision Concepts Demonstrated

### Matrix Operations
- Creation and manipulation of image matrices
- Element access and modification
- Mathematical operations (addition, multiplication)

### Image Processing
- **Filtering**: Blur, edge detection, morphological operations
- **Color Spaces**: BGR, RGB, HSV, Grayscale conversions
- **Enhancement**: Histogram equalization, contrast adjustment

### Feature Detection
- **Corners**: Harris corner detector
- **Edges**: Canny edge detection, Sobel operators
- **Blobs**: Connected component analysis

### Object Detection
- **Template Matching**: Finding patterns in images
- **Cascade Classifiers**: Haar-based face detection
- **Contour Analysis**: Shape detection and measurement

### Analysis Tools
- **Histograms**: Color distribution analysis
- **Statistics**: Min, max, mean calculations
- **Measurements**: Area, perimeter, bounding boxes

## Next Steps

1. **Complete the OpenCV build** to enable all examples
2. **Download test images** for more interesting results
3. **Add webcam support** for real-time processing
4. **Experiment with parameters** to understand algorithm behavior
5. **Create your own examples** based on these templates

## Troubleshooting

### "opencv2/opencv.hpp not found"
- Complete the OpenCV build: `./build_env.sh build-all`
- Check that headers are generated in `build/include/`

### "cascade file not found" 
- Download OpenCV data: `git clone https://github.com/opencv/opencv_extra`
- Copy cascade files to `data/haarcascades/`

### Slow performance
- Use Release build: `-O2` flag
- Enable CPU optimizations in OpenCV build
- Consider image size for processing

## Contributing

Feel free to add more examples! Follow the existing pattern:
- Document what the example demonstrates
- Include both generated and user-provided input options
- Add error handling and helpful output messages
- Test with the simple demo pattern first