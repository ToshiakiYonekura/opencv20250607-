# CMakeLists.txt for OpenCV Examples
cmake_minimum_required(VERSION 3.10)
project(OpenCV_Examples)

# Find OpenCV
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs highgui objdetect features2d)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Compiler options
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
endif()

# Basic CV Examples
add_executable(image_operations basic_cv/image_operations.cpp)
target_link_libraries(image_operations ${OpenCV_LIBS})

# Face Detection Examples
add_executable(face_detector face_detection/face_detector.cpp)
target_link_libraries(face_detector ${OpenCV_LIBS})

# Advanced CV Examples
add_executable(advanced_cv image_processing/advanced_cv.cpp)
target_link_libraries(advanced_cv ${OpenCV_LIBS})

# Install examples (optional)
install(TARGETS image_operations face_detector advanced_cv
        RUNTIME DESTINATION bin)

# Print OpenCV information
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Create a test target
enable_testing()

add_test(NAME test_image_operations 
         COMMAND image_operations
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_test(NAME test_advanced_cv 
         COMMAND advanced_cv
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Note: face_detector requires cascade files, so we don't include it in automatic tests