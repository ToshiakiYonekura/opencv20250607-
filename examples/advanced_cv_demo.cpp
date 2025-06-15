/**
 * @file advanced_cv_demo.cpp
 * @brief Advanced Computer Vision Concepts Demonstration
 * 
 * This example demonstrates real computer vision algorithms:
 * - Advanced image processing techniques
 * - Feature detection and matching
 * - Object recognition concepts
 * - Statistical image analysis
 * - Computer vision pipeline simulation
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>
#include <random>

using namespace std;

// Enhanced Matrix class with advanced CV operations
class CVMatrix {
private:
    vector<vector<int>> data;
    int rows, cols;

public:
    CVMatrix(int r, int c) : rows(r), cols(c) {
        data.resize(rows, vector<int>(cols, 0));
    }

    void set(int row, int col, int value) {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            data[row][col] = max(0, min(255, value));
        }
    }

    int get(int row, int col) const {
        if (row >= 0 && row < rows && col >= 0 && col < cols) {
            return data[row][col];
        }
        return 0;
    }

    void print() const {
        cout << "Matrix " << rows << "x" << cols << ":" << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                printf("%3d ", data[i][j]);
            }
            cout << endl;
        }
    }

    // Create synthetic images for testing
    static CVMatrix createCheckerboard(int rows, int cols, int square_size = 2) {
        CVMatrix img(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int block_i = i / square_size;
                int block_j = j / square_size;
                int value = ((block_i + block_j) % 2) ? 255 : 0;
                img.set(i, j, value);
            }
        }
        return img;
    }

    static CVMatrix createCircle(int rows, int cols, int center_x, int center_y, int radius) {
        CVMatrix img(rows, cols);
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int dx = j - center_x;
                int dy = i - center_y;
                int distance = sqrt(dx*dx + dy*dy);
                if (distance <= radius) {
                    img.set(i, j, 255 - (distance * 255 / radius));
                }
            }
        }
        return img;
    }

    static CVMatrix createNoisyImage(int rows, int cols, int noise_level = 50) {
        CVMatrix img(rows, cols);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(0, noise_level);
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int base_value = (i + j) * 128 / (rows + cols);
                int noise = dis(gen) - noise_level/2;
                img.set(i, j, base_value + noise);
            }
        }
        return img;
    }

    // Advanced filtering operations
    CVMatrix gaussianBlur(int kernel_size = 3) const {
        CVMatrix result(rows, cols);
        int half_kernel = kernel_size / 2;
        
        // Simple Gaussian approximation
        vector<vector<float>> kernel(kernel_size, vector<float>(kernel_size));
        float sigma = kernel_size / 3.0f;
        float sum = 0;
        
        for (int i = 0; i < kernel_size; i++) {
            for (int j = 0; j < kernel_size; j++) {
                int x = i - half_kernel;
                int y = j - half_kernel;
                kernel[i][j] = exp(-(x*x + y*y) / (2 * sigma * sigma));
                sum += kernel[i][j];
            }
        }
        
        // Normalize kernel
        for (int i = 0; i < kernel_size; i++) {
            for (int j = 0; j < kernel_size; j++) {
                kernel[i][j] /= sum;
            }
        }
        
        // Apply convolution
        for (int i = half_kernel; i < rows - half_kernel; i++) {
            for (int j = half_kernel; j < cols - half_kernel; j++) {
                float value = 0;
                for (int ki = 0; ki < kernel_size; ki++) {
                    for (int kj = 0; kj < kernel_size; kj++) {
                        value += get(i - half_kernel + ki, j - half_kernel + kj) * kernel[ki][kj];
                    }
                }
                result.set(i, j, (int)value);
            }
        }
        return result;
    }

    CVMatrix sobelEdgeDetection() const {
        CVMatrix result(rows, cols);
        
        // Sobel kernels
        int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
        int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
        
        for (int i = 1; i < rows - 1; i++) {
            for (int j = 1; j < cols - 1; j++) {
                int gx = 0, gy = 0;
                
                for (int ki = 0; ki < 3; ki++) {
                    for (int kj = 0; kj < 3; kj++) {
                        int pixel = get(i + ki - 1, j + kj - 1);
                        gx += pixel * sobel_x[ki][kj];
                        gy += pixel * sobel_y[ki][kj];
                    }
                }
                
                int magnitude = sqrt(gx*gx + gy*gy);
                result.set(i, j, min(255, magnitude));
            }
        }
        return result;
    }

    // Morphological operations
    CVMatrix dilate(int kernel_size = 3) const {
        CVMatrix result(rows, cols);
        int half_kernel = kernel_size / 2;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int max_val = 0;
                for (int ki = -half_kernel; ki <= half_kernel; ki++) {
                    for (int kj = -half_kernel; kj <= half_kernel; kj++) {
                        max_val = max(max_val, get(i + ki, j + kj));
                    }
                }
                result.set(i, j, max_val);
            }
        }
        return result;
    }

    CVMatrix erode(int kernel_size = 3) const {
        CVMatrix result(rows, cols);
        int half_kernel = kernel_size / 2;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int min_val = 255;
                for (int ki = -half_kernel; ki <= half_kernel; ki++) {
                    for (int kj = -half_kernel; kj <= half_kernel; kj++) {
                        min_val = min(min_val, get(i + ki, j + kj));
                    }
                }
                result.set(i, j, min_val);
            }
        }
        return result;
    }

    // Feature detection
    vector<pair<int, int>> harrisCornerDetection(float threshold = 50000) const {
        vector<pair<int, int>> corners;
        CVMatrix Ix(rows, cols), Iy(rows, cols);
        
        // Compute gradients
        for (int i = 1; i < rows - 1; i++) {
            for (int j = 1; j < cols - 1; j++) {
                Ix.set(i, j, get(i, j+1) - get(i, j-1));
                Iy.set(i, j, get(i+1, j) - get(i-1, j));
            }
        }
        
        // Harris corner response
        for (int i = 2; i < rows - 2; i++) {
            for (int j = 2; j < cols - 2; j++) {
                float Ixx = 0, Iyy = 0, Ixy = 0;
                
                // Sum over window
                for (int wi = -2; wi <= 2; wi++) {
                    for (int wj = -2; wj <= 2; wj++) {
                        float ix = Ix.get(i + wi, j + wj);
                        float iy = Iy.get(i + wi, j + wj);
                        Ixx += ix * ix;
                        Iyy += iy * iy;
                        Ixy += ix * iy;
                    }
                }
                
                // Harris response
                float det = Ixx * Iyy - Ixy * Ixy;
                float trace = Ixx + Iyy;
                float response = det - 0.04f * trace * trace;
                
                if (response > threshold) {
                    corners.push_back({i, j});
                }
            }
        }
        return corners;
    }

    // Template matching
    pair<int, int> templateMatch(const CVMatrix& templ) const {
        int best_x = 0, best_y = 0;
        float best_score = -1;
        
        for (int i = 0; i <= rows - templ.rows; i++) {
            for (int j = 0; j <= cols - templ.cols; j++) {
                float score = 0;
                float norm1 = 0, norm2 = 0;
                
                // Normalized cross correlation
                for (int ti = 0; ti < templ.rows; ti++) {
                    for (int tj = 0; tj < templ.cols; tj++) {
                        float img_val = get(i + ti, j + tj);
                        float templ_val = templ.get(ti, tj);
                        score += img_val * templ_val;
                        norm1 += img_val * img_val;
                        norm2 += templ_val * templ_val;
                    }
                }
                
                if (norm1 > 0 && norm2 > 0) {
                    score /= sqrt(norm1 * norm2);
                    if (score > best_score) {
                        best_score = score;
                        best_x = j;
                        best_y = i;
                    }
                }
            }
        }
        return {best_x, best_y};
    }

    // Statistical analysis
    struct ImageStats {
        int min_val, max_val;
        double mean, std_dev;
        vector<int> histogram;
    };

    ImageStats computeStatistics() const {
        ImageStats stats;
        stats.histogram.resize(256, 0);
        stats.min_val = 255;
        stats.max_val = 0;
        int sum = 0;
        
        // First pass: min, max, mean, histogram
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int val = get(i, j);
                stats.min_val = min(stats.min_val, val);
                stats.max_val = max(stats.max_val, val);
                sum += val;
                stats.histogram[val]++;
            }
        }
        
        stats.mean = (double)sum / (rows * cols);
        
        // Second pass: standard deviation
        double var_sum = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                double diff = get(i, j) - stats.mean;
                var_sum += diff * diff;
            }
        }
        stats.std_dev = sqrt(var_sum / (rows * cols));
        
        return stats;
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

// Advanced Computer Vision Pipeline
class AdvancedCVPipeline {
public:
    static void demonstrateImageGeneration() {
        cout << "=== Advanced Image Generation ===" << endl;
        
        // Create different types of test images
        cout << "Creating checkerboard pattern..." << endl;
        CVMatrix checkerboard = CVMatrix::createCheckerboard(8, 8, 2);
        checkerboard.print();
        
        cout << "\nCreating circle pattern..." << endl;
        CVMatrix circle = CVMatrix::createCircle(8, 8, 4, 4, 3);
        circle.print();
        
        cout << "\nCreating noisy image..." << endl;
        CVMatrix noisy = CVMatrix::createNoisyImage(6, 6, 30);
        noisy.print();
    }

    static void demonstrateAdvancedFiltering() {
        cout << "\n=== Advanced Filtering Techniques ===" << endl;
        
        CVMatrix noisy_img = CVMatrix::createNoisyImage(8, 8, 40);
        cout << "Original noisy image:" << endl;
        noisy_img.print();
        
        cout << "\nGaussian blur (noise reduction):" << endl;
        CVMatrix blurred = noisy_img.gaussianBlur(3);
        blurred.print();
        
        cout << "\nSobel edge detection:" << endl;
        CVMatrix edges = noisy_img.sobelEdgeDetection();
        edges.print();
    }

    static void demonstrateMorphologicalOperations() {
        cout << "\n=== Morphological Operations ===" << endl;
        
        CVMatrix binary_img = CVMatrix::createCheckerboard(8, 8, 1);
        cout << "Original binary image:" << endl;
        binary_img.print();
        
        cout << "\nDilation (expand white regions):" << endl;
        CVMatrix dilated = binary_img.dilate(3);
        dilated.print();
        
        cout << "\nErosion (shrink white regions):" << endl;
        CVMatrix eroded = binary_img.erode(3);
        eroded.print();
    }

    static void demonstrateFeatureDetection() {
        cout << "\n=== Feature Detection ===" << endl;
        
        // Create an image with corners
        CVMatrix test_img(10, 10);
        // Create a square
        for (int i = 3; i < 7; i++) {
            for (int j = 3; j < 7; j++) {
                if (i == 3 || i == 6 || j == 3 || j == 6) {
                    test_img.set(i, j, 255);
                }
            }
        }
        
        cout << "Test image with square:" << endl;
        test_img.print();
        
        cout << "\nDetecting Harris corners..." << endl;
        vector<pair<int, int>> corners = test_img.harrisCornerDetection(1000);
        
        cout << "Found " << corners.size() << " corners at positions:" << endl;
        for (const auto& corner : corners) {
            cout << "(" << corner.second << ", " << corner.first << ") ";
        }
        cout << endl;
    }

    static void demonstrateTemplateMatching() {
        cout << "\n=== Template Matching ===" << endl;
        
        // Create a larger image with a pattern
        CVMatrix large_img = CVMatrix::createCheckerboard(12, 12, 2);
        cout << "Large image:" << endl;
        large_img.print();
        
        // Create a small template
        CVMatrix template_img = CVMatrix::createCheckerboard(4, 4, 2);
        cout << "\nTemplate to find:" << endl;
        template_img.print();
        
        // Find template in image
        auto match_pos = large_img.templateMatch(template_img);
        cout << "\nBest match found at position: (" << match_pos.first 
             << ", " << match_pos.second << ")" << endl;
    }

    static void demonstrateStatisticalAnalysis() {
        cout << "\n=== Statistical Image Analysis ===" << endl;
        
        CVMatrix test_img = CVMatrix::createNoisyImage(10, 10, 50);
        cout << "Test image:" << endl;
        test_img.print();
        
        auto stats = test_img.computeStatistics();
        cout << "\nImage Statistics:" << endl;
        cout << "- Size: " << test_img.getRows() << "x" << test_img.getCols() << endl;
        cout << "- Min value: " << stats.min_val << endl;
        cout << "- Max value: " << stats.max_val << endl;
        cout << "- Mean: " << stats.mean << endl;
        cout << "- Standard deviation: " << stats.std_dev << endl;
        cout << "- Dynamic range: " << (stats.max_val - stats.min_val) << endl;
        
        // Show histogram peaks
        cout << "\nHistogram peaks (intensity -> count):" << endl;
        for (int i = 0; i < 256; i += 32) {
            int count = 0;
            for (int j = i; j < min(256, i + 32); j++) {
                count += stats.histogram[j];
            }
            if (count > 0) {
                cout << i << "-" << (i+31) << ": " << count << " pixels" << endl;
            }
        }
    }

    static void demonstrateComputerVisionPipeline() {
        cout << "\n=== Complete Computer Vision Pipeline ===" << endl;
        
        cout << "Step 1: Image Acquisition (synthetic)" << endl;
        CVMatrix input_img = CVMatrix::createNoisyImage(12, 12, 30);
        
        cout << "Step 2: Preprocessing - Noise reduction" << endl;
        CVMatrix denoised = input_img.gaussianBlur(3);
        
        cout << "Step 3: Feature extraction - Edge detection" << endl;
        CVMatrix edges = denoised.sobelEdgeDetection();
        
        cout << "Step 4: Morphological processing" << endl;
        CVMatrix processed = edges.dilate(3).erode(3);  // Closing operation
        
        cout << "Step 5: Feature detection" << endl;
        vector<pair<int, int>> features = processed.harrisCornerDetection(500);
        
        cout << "Step 6: Analysis and results" << endl;
        auto stats = processed.computeStatistics();
        
        cout << "\nPipeline Results:" << endl;
        cout << "- Input image: " << input_img.getRows() << "x" << input_img.getCols() << endl;
        cout << "- Features detected: " << features.size() << endl;
        cout << "- Processed image statistics:" << endl;
        cout << "  * Mean intensity: " << stats.mean << endl;
        cout << "  * Edge strength (std dev): " << stats.std_dev << endl;
        
        cout << "\nFinal processed image:" << endl;
        processed.print();
    }

    static void demonstrateRealWorldApplications() {
        cout << "\n=== Real-World Applications Simulation ===" << endl;
        
        cout << "1. Quality Control Inspection:" << endl;
        CVMatrix product = CVMatrix::createCircle(8, 8, 4, 4, 3);
        auto defects = product.harrisCornerDetection(1000);
        cout << "   - Defects detected: " << defects.size() << endl;
        
        cout << "\n2. Medical Image Analysis:" << endl;
        CVMatrix medical_img = CVMatrix::createNoisyImage(8, 8, 20);
        CVMatrix enhanced = medical_img.gaussianBlur(3);
        auto med_stats = enhanced.computeStatistics();
        cout << "   - Image contrast: " << (med_stats.max_val - med_stats.min_val) << endl;
        cout << "   - Tissue density (mean): " << med_stats.mean << endl;
        
        cout << "\n3. Autonomous Vehicle Vision:" << endl;
        CVMatrix road_img = CVMatrix::createCheckerboard(10, 10, 2);
        CVMatrix lane_edges = road_img.sobelEdgeDetection();
        auto lane_stats = lane_edges.computeStatistics();
        cout << "   - Lane markings detected (edge strength): " << lane_stats.mean << endl;
        
        cout << "\n4. Security Camera Analysis:" << endl;
        CVMatrix camera_frame = CVMatrix::createNoisyImage(10, 10, 40);
        CVMatrix motion_edges = camera_frame.sobelEdgeDetection();
        auto motion_features = motion_edges.harrisCornerDetection(2000);
        cout << "   - Motion features: " << motion_features.size() << endl;
        cout << "   - Activity level: " << (motion_features.size() > 5 ? "High" : "Low") << endl;
    }
};

int main() {
    cout << "Advanced Computer Vision Concepts Demonstration" << endl;
    cout << "===============================================" << endl;
    
    try {
        // Demonstrate image generation
        AdvancedCVPipeline::demonstrateImageGeneration();
        
        // Demonstrate advanced filtering
        AdvancedCVPipeline::demonstrateAdvancedFiltering();
        
        // Demonstrate morphological operations
        AdvancedCVPipeline::demonstrateMorphologicalOperations();
        
        // Demonstrate feature detection
        AdvancedCVPipeline::demonstrateFeatureDetection();
        
        // Demonstrate template matching
        AdvancedCVPipeline::demonstrateTemplateMatching();
        
        // Demonstrate statistical analysis
        AdvancedCVPipeline::demonstrateStatisticalAnalysis();
        
        // Demonstrate complete CV pipeline
        AdvancedCVPipeline::demonstrateComputerVisionPipeline();
        
        // Demonstrate real-world applications
        AdvancedCVPipeline::demonstrateRealWorldApplications();
        
        cout << "\n=== Summary ===" << endl;
        cout << "This advanced demo showcased:" << endl;
        cout << "✓ Image preprocessing and noise reduction" << endl;
        cout << "✓ Advanced filtering (Gaussian, Sobel)" << endl;
        cout << "✓ Morphological operations (dilation, erosion)" << endl;
        cout << "✓ Feature detection (Harris corners)" << endl;
        cout << "✓ Template matching and pattern recognition" << endl;
        cout << "✓ Statistical image analysis" << endl;
        cout << "✓ Complete computer vision pipeline" << endl;
        cout << "✓ Real-world application simulations" << endl;
        
        cout << "\nWith full OpenCV, these concepts work with:" << endl;
        cout << "• High-resolution images and video streams" << endl;
        cout << "• Real camera input and file I/O" << endl;
        cout << "• GPU acceleration and optimization" << endl;
        cout << "• Machine learning integration" << endl;
        cout << "• Advanced algorithms (SIFT, SURF, ORB)" << endl;
        
        return 0;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
}