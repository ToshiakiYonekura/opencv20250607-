/**
 * @file image_operations.cpp
 * @brief Basic OpenCV image operations demonstration
 * 
 * This example demonstrates:
 * - Loading and saving images
 * - Basic image transformations
 * - Color space conversions
 * - Image filtering and enhancement
 */

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace cv;
using namespace std;

class ImageProcessor {
public:
    /**
     * @brief Load an image from file
     * @param filepath Path to the image file
     * @return Loaded image as Mat object
     */
    static Mat loadImage(const string& filepath) {
        Mat image = imread(filepath, IMREAD_COLOR);
        if (image.empty()) {
            throw runtime_error("Could not load image: " + filepath);
        }
        return image;
    }

    /**
     * @brief Create a simple test image for demonstration
     * @param width Image width
     * @param height Image height
     * @return Generated test image
     */
    static Mat createTestImage(int width = 640, int height = 480) {
        Mat image = Mat::zeros(height, width, CV_8UC3);
        
        // Create a gradient background
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                image.at<Vec3b>(y, x) = Vec3b(
                    (x * 255) / width,           // Blue gradient
                    (y * 255) / height,          // Green gradient
                    ((x + y) * 255) / (width + height)  // Red gradient
                );
            }
        }
        
        // Add some geometric shapes
        circle(image, Point(width/4, height/4), 50, Scalar(0, 255, 255), -1);
        rectangle(image, Point(width/2, height/4), Point(width/2 + 100, height/4 + 80), 
                 Scalar(255, 0, 255), -1);
        ellipse(image, Point(3*width/4, height/4), Size(60, 40), 45, 0, 360, 
                Scalar(255, 255, 0), -1);
        
        // Add text
        putText(image, "OpenCV Demo", Point(width/4, 3*height/4), 
                FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
        
        return image;
    }

    /**
     * @brief Apply various image processing operations
     * @param input Input image
     * @return Processed images in a vector
     */
    static vector<Mat> processImage(const Mat& input) {
        vector<Mat> results;
        
        // 1. Original image
        results.push_back(input.clone());
        
        // 2. Grayscale conversion
        Mat gray;
        cvtColor(input, gray, COLOR_BGR2GRAY);
        Mat gray_bgr;
        cvtColor(gray, gray_bgr, COLOR_GRAY2BGR);
        results.push_back(gray_bgr);
        
        // 3. HSV color space
        Mat hsv;
        cvtColor(input, hsv, COLOR_BGR2HSV);
        results.push_back(hsv);
        
        // 4. Gaussian blur
        Mat blurred;
        GaussianBlur(input, blurred, Size(15, 15), 0);
        results.push_back(blurred);
        
        // 5. Edge detection
        Mat edges;
        Canny(gray, edges, 100, 200);
        cvtColor(edges, edges, COLOR_GRAY2BGR);
        results.push_back(edges);
        
        // 6. Histogram equalization
        Mat equalized;
        vector<Mat> channels;
        cvtColor(input, hsv, COLOR_BGR2HSV);
        split(hsv, channels);
        equalizeHist(channels[2], channels[2]);
        merge(channels, hsv);
        cvtColor(hsv, equalized, COLOR_HSV2BGR);
        results.push_back(equalized);
        
        return results;
    }

    /**
     * @brief Create a mosaic of multiple images
     * @param images Vector of images to combine
     * @param cols Number of columns in the mosaic
     * @return Combined mosaic image
     */
    static Mat createMosaic(const vector<Mat>& images, int cols = 3) {
        if (images.empty()) return Mat();
        
        int rows = (images.size() + cols - 1) / cols;
        int img_width = images[0].cols;
        int img_height = images[0].rows;
        
        Mat mosaic = Mat::zeros(rows * img_height, cols * img_width, images[0].type());
        
        for (size_t i = 0; i < images.size(); i++) {
            int row = i / cols;
            int col = i % cols;
            
            Rect roi(col * img_width, row * img_height, img_width, img_height);
            
            if (images[i].size() == images[0].size() && images[i].type() == images[0].type()) {
                images[i].copyTo(mosaic(roi));
            } else {
                Mat resized;
                resize(images[i], resized, Size(img_width, img_height));
                if (resized.type() != images[0].type()) {
                    if (resized.channels() == 1 && images[0].channels() == 3) {
                        cvtColor(resized, resized, COLOR_GRAY2BGR);
                    }
                }
                resized.copyTo(mosaic(roi));
            }
        }
        
        return mosaic;
    }
};

int main(int argc, char** argv) {
    try {
        cout << "OpenCV Basic Image Operations Demo" << endl;
        cout << "==================================" << endl;
        
        // Create or load test image
        Mat image;
        if (argc > 1) {
            cout << "Loading image: " << argv[1] << endl;
            image = ImageProcessor::loadImage(argv[1]);
        } else {
            cout << "Creating test image..." << endl;
            image = ImageProcessor::createTestImage();
        }
        
        cout << "Image size: " << image.cols << "x" << image.rows << endl;
        cout << "Image channels: " << image.channels() << endl;
        
        // Process the image
        cout << "Processing image..." << endl;
        vector<Mat> processed = ImageProcessor::processImage(image);
        
        // Add labels to images
        vector<string> labels = {
            "Original", "Grayscale", "HSV", 
            "Blurred", "Edges", "Equalized"
        };
        
        for (size_t i = 0; i < processed.size() && i < labels.size(); i++) {
            putText(processed[i], labels[i], Point(10, 30), 
                   FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 2);
        }
        
        // Create mosaic
        cout << "Creating mosaic..." << endl;
        Mat mosaic = ImageProcessor::createMosaic(processed, 3);
        
        // Save results
        cout << "Saving results..." << endl;
        imwrite("output_original.jpg", image);
        imwrite("output_mosaic.jpg", mosaic);
        
        cout << "Results saved:" << endl;
        cout << "- output_original.jpg (original image)" << endl;
        cout << "- output_mosaic.jpg (processing results)" << endl;
        
        // Print some image statistics
        Scalar mean_val = mean(image);
        cout << endl << "Image Statistics:" << endl;
        cout << "Mean BGR values: " << mean_val << endl;
        
        double min_val, max_val;
        minMaxLoc(image, &min_val, &max_val);
        cout << "Min/Max intensity: " << min_val << "/" << max_val << endl;
        
        return 0;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
}