/**
 * @file advanced_cv.cpp
 * @brief Advanced computer vision operations
 * 
 * This example demonstrates:
 * - Contour detection and analysis
 * - Feature detection (corners, blobs)
 * - Morphological operations
 * - Template matching
 * - Histogram analysis
 */

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <algorithm>

using namespace cv;
using namespace std;

class AdvancedCV {
public:
    /**
     * @brief Detect and analyze contours in an image
     * @param image Input image
     * @return Image with drawn contours and analysis
     */
    static Mat analyzeContours(const Mat& image) {
        Mat gray, binary, result;
        
        // Convert to grayscale
        cvtColor(image, gray, COLOR_BGR2GRAY);
        
        // Apply threshold to create binary image
        threshold(gray, binary, 127, 255, THRESH_BINARY);
        
        // Find contours
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(binary, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        
        // Create result image
        result = image.clone();
        
        // Analyze and draw contours
        for (size_t i = 0; i < contours.size(); i++) {
            // Calculate contour properties
            double area = contourArea(contours[i]);
            double perimeter = arcLength(contours[i], true);
            
            // Skip small contours
            if (area < 100) continue;
            
            // Draw contour
            Scalar color(rand() % 256, rand() % 256, rand() % 256);
            drawContours(result, contours, (int)i, color, 2);
            
            // Get bounding rectangle
            Rect bounding_rect = boundingRect(contours[i]);
            rectangle(result, bounding_rect, color, 1);
            
            // Get minimum enclosing circle
            Point2f center;
            float radius;
            minEnclosingCircle(contours[i], center, radius);
            circle(result, center, (int)radius, color, 1);
            
            // Approximate contour to polygon
            vector<Point> approx;
            approxPolyDP(contours[i], approx, 0.02 * perimeter, true);
            
            // Add label with information
            Point label_pos(bounding_rect.x, bounding_rect.y - 10);
            string label = "A:" + to_string((int)area) + " V:" + to_string(approx.size());
            putText(result, label, label_pos, FONT_HERSHEY_SIMPLEX, 0.4, color, 1);
        }
        
        // Add summary
        string summary = "Contours: " + to_string(contours.size());
        putText(result, summary, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, 
               Scalar(255, 255, 255), 2);
        
        return result;
    }

    /**
     * @brief Detect corners using Harris corner detector
     * @param image Input image
     * @return Image with detected corners
     */
    static Mat detectCorners(const Mat& image) {
        Mat gray, corners, result;
        
        // Convert to grayscale
        cvtColor(image, gray, COLOR_BGR2GRAY);
        
        // Harris corner detection
        cornerHarris(gray, corners, 2, 3, 0.04);
        
        // Normalize corner response
        normalize(corners, corners, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
        
        // Create result image
        result = image.clone();
        
        // Draw corners
        int corner_count = 0;
        for (int i = 0; i < corners.rows; i++) {
            for (int j = 0; j < corners.cols; j++) {
                if (corners.at<float>(i, j) > 100) {  // Threshold for corner detection
                    circle(result, Point(j, i), 3, Scalar(0, 255, 0), -1);
                    corner_count++;
                }
            }
        }
        
        // Add summary
        string summary = "Corners: " + to_string(corner_count);
        putText(result, summary, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, 
               Scalar(0, 255, 0), 2);
        
        return result;
    }

    /**
     * @brief Apply morphological operations
     * @param image Input image
     * @return Images with different morphological operations
     */
    static vector<Mat> morphologicalOperations(const Mat& image) {
        Mat gray, binary;
        vector<Mat> results;
        
        // Convert to grayscale and threshold
        cvtColor(image, gray, COLOR_BGR2GRAY);
        threshold(gray, binary, 127, 255, THRESH_BINARY);
        
        // Define morphological kernel
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
        
        // Original binary
        Mat binary_colored;
        cvtColor(binary, binary_colored, COLOR_GRAY2BGR);
        results.push_back(binary_colored);
        
        // Erosion
        Mat eroded;
        erode(binary, eroded, kernel);
        cvtColor(eroded, eroded, COLOR_GRAY2BGR);
        results.push_back(eroded);
        
        // Dilation
        Mat dilated;
        dilate(binary, dilated, kernel);
        cvtColor(dilated, dilated, COLOR_GRAY2BGR);
        results.push_back(dilated);
        
        // Opening (erosion followed by dilation)
        Mat opened;
        morphologyEx(binary, opened, MORPH_OPEN, kernel);
        cvtColor(opened, opened, COLOR_GRAY2BGR);
        results.push_back(opened);
        
        // Closing (dilation followed by erosion)
        Mat closed;
        morphologyEx(binary, closed, MORPH_CLOSE, kernel);
        cvtColor(closed, closed, COLOR_GRAY2BGR);
        results.push_back(closed);
        
        // Gradient (difference between dilation and erosion)
        Mat gradient;
        morphologyEx(binary, gradient, MORPH_GRADIENT, kernel);
        cvtColor(gradient, gradient, COLOR_GRAY2BGR);
        results.push_back(gradient);
        
        // Add labels
        vector<string> labels = {"Binary", "Eroded", "Dilated", "Opened", "Closed", "Gradient"};
        for (size_t i = 0; i < results.size() && i < labels.size(); i++) {
            putText(results[i], labels[i], Point(10, 30), FONT_HERSHEY_SIMPLEX, 
                   0.7, Scalar(255, 255, 255), 2);
        }
        
        return results;
    }

    /**
     * @brief Perform template matching
     * @param image Input image
     * @param templ Template image
     * @return Image with matching results
     */
    static Mat templateMatching(const Mat& image, const Mat& templ) {
        Mat result, match_result;
        
        // Perform template matching
        matchTemplate(image, templ, match_result, TM_CCOEFF_NORMED);
        
        // Find best match location
        double min_val, max_val;
        Point min_loc, max_loc;
        minMaxLoc(match_result, &min_val, &max_val, &min_loc, &max_loc);
        
        // Create result image
        result = image.clone();
        
        // Draw rectangle around best match
        Rect match_rect(max_loc, templ.size());
        rectangle(result, match_rect, Scalar(0, 255, 0), 2);
        
        // Add match confidence
        string confidence = "Match: " + to_string((int)(max_val * 100)) + "%";
        putText(result, confidence, Point(max_loc.x, max_loc.y - 10), 
               FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
        
        return result;
    }

    /**
     * @brief Analyze color histogram
     * @param image Input image
     * @return Histogram visualization
     */
    static Mat analyzeHistogram(const Mat& image) {
        vector<Mat> bgr_planes;
        split(image, bgr_planes);
        
        // Set histogram parameters
        int hist_size = 256;
        float range[] = {0, 256};
        const float* hist_range = {range};
        
        // Calculate histograms
        Mat b_hist, g_hist, r_hist;
        calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &hist_size, &hist_range);
        calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &hist_size, &hist_range);
        calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &hist_size, &hist_range);
        
        // Create histogram image
        int hist_w = 512, hist_h = 400;
        int bin_w = cvRound((double)hist_w / hist_size);
        Mat hist_image(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
        
        // Normalize histograms
        normalize(b_hist, b_hist, 0, hist_image.rows, NORM_MINMAX, -1, Mat());
        normalize(g_hist, g_hist, 0, hist_image.rows, NORM_MINMAX, -1, Mat());
        normalize(r_hist, r_hist, 0, hist_image.rows, NORM_MINMAX, -1, Mat());
        
        // Draw histogram
        for (int i = 1; i < hist_size; i++) {
            line(hist_image,
                 Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
                 Point(bin_w * i, hist_h - cvRound(b_hist.at<float>(i))),
                 Scalar(255, 0, 0), 2, 8, 0);
            line(hist_image,
                 Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
                 Point(bin_w * i, hist_h - cvRound(g_hist.at<float>(i))),
                 Scalar(0, 255, 0), 2, 8, 0);
            line(hist_image,
                 Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
                 Point(bin_w * i, hist_h - cvRound(r_hist.at<float>(i))),
                 Scalar(0, 0, 255), 2, 8, 0);
        }
        
        // Add labels
        putText(hist_image, "Blue", Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0), 2);
        putText(hist_image, "Green", Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
        putText(hist_image, "Red", Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
        
        return hist_image;
    }

    /**
     * @brief Create a comprehensive test image
     * @return Generated test image with various shapes and patterns
     */
    static Mat createAdvancedTestImage() {
        Mat image = Mat::zeros(500, 700, CV_8UC3);
        
        // Background gradient
        for (int y = 0; y < image.rows; y++) {
            for (int x = 0; x < image.cols; x++) {
                int intensity = (x + y) * 128 / (image.cols + image.rows);
                image.at<Vec3b>(y, x) = Vec3b(intensity/3, intensity/2, intensity);
            }
        }
        
        // Add various shapes for contour detection
        rectangle(image, Point(50, 50), Point(150, 120), Scalar(255, 0, 0), -1);
        circle(image, Point(250, 100), 40, Scalar(0, 255, 0), -1);
        
        // Triangle
        vector<Point> triangle;
        triangle.push_back(Point(350, 50));
        triangle.push_back(Point(320, 120));
        triangle.push_back(Point(380, 120));
        fillPoly(image, triangle, Scalar(0, 0, 255));
        
        // Complex polygon
        vector<Point> polygon;
        polygon.push_back(Point(450, 80));
        polygon.push_back(Point(480, 60));
        polygon.push_back(Point(520, 80));
        polygon.push_back(Point(510, 120));
        polygon.push_back(Point(470, 120));
        fillPoly(image, polygon, Scalar(255, 255, 0));
        
        // Add some noise and texture
        Mat noise = Mat::zeros(image.size(), CV_8UC3);
        randu(noise, Scalar(0, 0, 0), Scalar(50, 50, 50));
        add(image, noise, image);
        
        // Add corners by drawing lines
        line(image, Point(100, 300), Point(200, 300), Scalar(255, 255, 255), 3);
        line(image, Point(200, 300), Point(200, 400), Scalar(255, 255, 255), 3);
        line(image, Point(300, 350), Point(400, 300), Scalar(255, 255, 255), 3);
        line(image, Point(400, 300), Point(450, 400), Scalar(255, 255, 255), 3);
        
        return image;
    }
};

int main(int argc, char** argv) {
    try {
        cout << "OpenCV Advanced Computer Vision Demo" << endl;
        cout << "====================================" << endl;
        
        // Load or create test image
        Mat image;
        if (argc > 1) {
            cout << "Loading image: " << argv[1] << endl;
            image = imread(argv[1], IMREAD_COLOR);
            if (image.empty()) {
                cerr << "Could not load image: " << argv[1] << endl;
                return -1;
            }
        } else {
            cout << "Creating advanced test image..." << endl;
            image = AdvancedCV::createAdvancedTestImage();
        }
        
        cout << "Processing image..." << endl;
        
        // 1. Contour analysis
        cout << "Analyzing contours..." << endl;
        Mat contour_result = AdvancedCV::analyzeContours(image);
        
        // 2. Corner detection
        cout << "Detecting corners..." << endl;
        Mat corner_result = AdvancedCV::detectCorners(image);
        
        // 3. Morphological operations
        cout << "Applying morphological operations..." << endl;
        vector<Mat> morph_results = AdvancedCV::morphologicalOperations(image);
        
        // 4. Histogram analysis
        cout << "Analyzing histogram..." << endl;
        Mat hist_result = AdvancedCV::analyzeHistogram(image);
        
        // Save results
        cout << "Saving results..." << endl;
        imwrite("output_original_advanced.jpg", image);
        imwrite("output_contours.jpg", contour_result);
        imwrite("output_corners.jpg", corner_result);
        imwrite("output_histogram.jpg", hist_result);
        
        // Save morphological operations
        vector<string> morph_names = {"binary", "eroded", "dilated", "opened", "closed", "gradient"};
        for (size_t i = 0; i < morph_results.size() && i < morph_names.size(); i++) {
            string filename = "output_morph_" + morph_names[i] + ".jpg";
            imwrite(filename, morph_results[i]);
        }
        
        cout << endl << "Results saved:" << endl;
        cout << "- output_original_advanced.jpg (original image)" << endl;
        cout << "- output_contours.jpg (contour analysis)" << endl;
        cout << "- output_corners.jpg (corner detection)" << endl;
        cout << "- output_histogram.jpg (color histogram)" << endl;
        cout << "- output_morph_*.jpg (morphological operations)" << endl;
        
        return 0;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
}