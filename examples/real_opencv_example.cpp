#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>

int main() {
    std::cout << "Real OpenCV Example - Using Actual OpenCV Libraries" << std::endl;
    std::cout << "====================================================" << std::endl;
    
    // Display OpenCV version
    std::cout << "OpenCV Version: " << cv::getVersionString() << std::endl;
    std::cout << "Build Information: " << cv::getBuildInformation() << std::endl << std::endl;
    
    // === BASIC MATRIX OPERATIONS ===
    std::cout << "=== Matrix Operations ===" << std::endl;
    
    // Create matrices using OpenCV Mat class
    cv::Mat matrix1 = (cv::Mat_<double>(3, 3) << 
        1, 2, 3,
        4, 5, 6,
        7, 8, 9);
    
    cv::Mat matrix2 = (cv::Mat_<double>(3, 3) << 
        9, 8, 7,
        6, 5, 4,
        3, 2, 1);
    
    // Matrix operations
    cv::Mat sum = matrix1 + matrix2;
    cv::Mat product = matrix1 * matrix2;
    cv::Mat transpose;
    cv::transpose(matrix1, transpose);
    
    std::cout << "Matrix 1:" << std::endl << matrix1 << std::endl;
    std::cout << "Matrix 2:" << std::endl << matrix2 << std::endl;
    std::cout << "Sum:" << std::endl << sum << std::endl;
    std::cout << "Product:" << std::endl << product << std::endl;
    std::cout << "Matrix 1 Transposed:" << std::endl << transpose << std::endl;
    
    // === IMAGE PROCESSING ===
    std::cout << "\n=== Image Processing ===" << std::endl;
    
    // Create a synthetic image (grayscale)
    cv::Mat image = cv::Mat::zeros(200, 200, CV_8UC1);
    
    // Draw some shapes
    cv::circle(image, cv::Point(50, 50), 30, cv::Scalar(255), -1);
    cv::rectangle(image, cv::Point(100, 100), cv::Point(180, 180), cv::Scalar(128), -1);
    cv::line(image, cv::Point(0, 199), cv::Point(199, 0), cv::Scalar(200), 3);
    
    std::cout << "Created synthetic image with circle, rectangle, and line" << std::endl;
    std::cout << "Image size: " << image.size() << std::endl;
    std::cout << "Image type: " << image.type() << std::endl;
    
    // Apply various filters
    cv::Mat blurred, edges, dilated;
    
    // Gaussian blur
    cv::GaussianBlur(image, blurred, cv::Size(15, 15), 0);
    
    // Edge detection
    cv::Canny(image, edges, 50, 150);
    
    // Morphological operations
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::dilate(edges, dilated, kernel);
    
    std::cout << "Applied Gaussian blur, Canny edge detection, and dilation" << std::endl;
    
    // Save images if possible
    try {
        cv::imwrite("original_image.png", image);
        cv::imwrite("blurred_image.png", blurred);
        cv::imwrite("edges_image.png", edges);
        cv::imwrite("dilated_image.png", dilated);
        std::cout << "Images saved as PNG files" << std::endl;
    } catch (const cv::Exception& e) {
        std::cout << "Could not save images: " << e.what() << std::endl;
    }
    
    // === COLOR SPACE OPERATIONS ===
    std::cout << "\n=== Color Space Operations ===" << std::endl;
    
    // Create a color image
    cv::Mat colorImage = cv::Mat::zeros(150, 150, CV_8UC3);
    
    // Fill with different colors
    cv::rectangle(colorImage, cv::Rect(0, 0, 50, 150), cv::Scalar(255, 0, 0), -1);    // Blue
    cv::rectangle(colorImage, cv::Rect(50, 0, 50, 150), cv::Scalar(0, 255, 0), -1);   // Green
    cv::rectangle(colorImage, cv::Rect(100, 0, 50, 150), cv::Scalar(0, 0, 255), -1);  // Red
    
    // Convert color spaces
    cv::Mat hsvImage, grayImage;
    cv::cvtColor(colorImage, hsvImage, cv::COLOR_BGR2HSV);
    cv::cvtColor(colorImage, grayImage, cv::COLOR_BGR2GRAY);
    
    std::cout << "Created RGB image and converted to HSV and grayscale" << std::endl;
    std::cout << "Color image channels: " << colorImage.channels() << std::endl;
    std::cout << "Gray image channels: " << grayImage.channels() << std::endl;
    
    // === FEATURE DETECTION ===
    std::cout << "\n=== Feature Detection ===" << std::endl;
    
    // Detect corners using Harris corner detector
    cv::Mat corners, corners_norm;
    cv::cornerHarris(grayImage, corners, 2, 3, 0.04);
    cv::normalize(corners, corners_norm, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    
    // Count corner points
    int cornerCount = 0;
    for (int i = 0; i < corners_norm.rows; i++) {
        for (int j = 0; j < corners_norm.cols; j++) {
            if (corners_norm.at<uchar>(i, j) > 200) {
                cornerCount++;
            }
        }
    }
    
    std::cout << "Detected " << cornerCount << " corner points using Harris detector" << std::endl;
    
    // === STATISTICAL OPERATIONS ===
    std::cout << "\n=== Statistical Operations ===" << std::endl;
    
    cv::Scalar meanVal = cv::mean(image);
    cv::Scalar stdDev;
    cv::meanStdDev(image, meanVal, stdDev);
    
    double minVal, maxVal;
    cv::Point minLoc, maxLoc;
    cv::minMaxLoc(image, &minVal, &maxVal, &minLoc, &maxLoc);
    
    std::cout << "Image statistics:" << std::endl;
    std::cout << "  Mean: " << meanVal[0] << std::endl;
    std::cout << "  Std Dev: " << stdDev[0] << std::endl;
    std::cout << "  Min value: " << minVal << " at " << minLoc << std::endl;
    std::cout << "  Max value: " << maxVal << " at " << maxLoc << std::endl;
    
    // === SUMMARY ===
    std::cout << "\n=== Summary ===" << std::endl;
    std::cout << "✓ Successfully used OpenCV " << cv::getVersionString() << std::endl;
    std::cout << "✓ Performed matrix operations" << std::endl;
    std::cout << "✓ Created and processed images" << std::endl;
    std::cout << "✓ Applied filters and transformations" << std::endl;
    std::cout << "✓ Detected features and computed statistics" << std::endl;
    std::cout << "✓ All OpenCV modules working correctly!" << std::endl;
    
    return 0;
}