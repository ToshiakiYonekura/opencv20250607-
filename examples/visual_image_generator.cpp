/**
 * @file visual_image_generator.cpp
 * @brief Generate Visual Examples as Actual Images
 * 
 * This program creates visual demonstrations of computer vision concepts
 * and saves them as image files that you can view and analyze.
 */

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <fstream>

using namespace std;

// Simple image class for creating visual examples
class ImageGenerator {
private:
    vector<vector<vector<int>>> data; // RGB data
    int width, height;

public:
    ImageGenerator(int w, int h) : width(w), height(h) {
        data.resize(height, vector<vector<int>>(width, vector<int>(3, 0)));
    }

    void setPixel(int x, int y, int r, int g, int b) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            data[y][x][0] = max(0, min(255, r));
            data[y][x][1] = max(0, min(255, g));
            data[y][x][2] = max(0, min(255, b));
        }
    }

    void setPixelGray(int x, int y, int gray) {
        setPixel(x, y, gray, gray, gray);
    }

    int getPixelGray(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return (data[y][x][0] + data[y][x][1] + data[y][x][2]) / 3;
        }
        return 0;
    }

    // Save as PPM format (simple, viewable format)
    bool savePPM(const string& filename) const {
        ofstream file(filename);
        if (!file.is_open()) return false;

        file << "P3\n";
        file << width << " " << height << "\n";
        file << "255\n";

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file << data[y][x][0] << " " 
                     << data[y][x][1] << " " 
                     << data[y][x][2] << " ";
            }
            file << "\n";
        }
        return true;
    }

    // Draw basic shapes
    void drawLine(int x1, int y1, int x2, int y2, int r, int g, int b) {
        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (true) {
            setPixel(x1, y1, r, g, b);
            if (x1 == x2 && y1 == y2) break;
            int e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x1 += sx; }
            if (e2 < dx) { err += dx; y1 += sy; }
        }
    }

    void drawCircle(int cx, int cy, int radius, int r, int g, int b, bool filled = false) {
        for (int x = cx - radius; x <= cx + radius; x++) {
            for (int y = cy - radius; y <= cy + radius; y++) {
                int dx = x - cx;
                int dy = y - cy;
                int distance = sqrt(dx*dx + dy*dy);
                
                if (filled && distance <= radius) {
                    setPixel(x, y, r, g, b);
                } else if (!filled && abs(distance - radius) <= 1) {
                    setPixel(x, y, r, g, b);
                }
            }
        }
    }

    void drawRectangle(int x1, int y1, int x2, int y2, int r, int g, int b, bool filled = false) {
        if (filled) {
            for (int y = y1; y <= y2; y++) {
                for (int x = x1; x <= x2; x++) {
                    setPixel(x, y, r, g, b);
                }
            }
        } else {
            drawLine(x1, y1, x2, y1, r, g, b); // Top
            drawLine(x1, y2, x2, y2, r, g, b); // Bottom
            drawLine(x1, y1, x1, y2, r, g, b); // Left
            drawLine(x2, y1, x2, y2, r, g, b); // Right
        }
    }

    void fillBackground(int r, int g, int b) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                setPixel(x, y, r, g, b);
            }
        }
    }

    void addNoise(int strength) {
        srand(12345); // Fixed seed for reproducible results
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int noise = (rand() % (2 * strength + 1)) - strength;
                int r = data[y][x][0] + noise;
                int g = data[y][x][1] + noise;
                int b = data[y][x][2] + noise;
                setPixel(x, y, r, g, b);
            }
        }
    }

    // Image processing operations
    ImageGenerator gaussianBlur() const {
        ImageGenerator result(width, height);
        
        // Simple 3x3 Gaussian kernel
        int kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
        int kernel_sum = 16;
        
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                int r = 0, g = 0, b = 0;
                
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int weight = kernel[ky + 1][kx + 1];
                        r += data[y + ky][x + kx][0] * weight;
                        g += data[y + ky][x + kx][1] * weight;
                        b += data[y + ky][x + kx][2] * weight;
                    }
                }
                
                result.setPixel(x, y, r / kernel_sum, g / kernel_sum, b / kernel_sum);
            }
        }
        return result;
    }

    ImageGenerator edgeDetection() const {
        ImageGenerator result(width, height);
        
        // Sobel kernels
        int sobel_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
        int sobel_y[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
        
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                int gx = 0, gy = 0;
                
                for (int ky = -1; ky <= 1; ky++) {
                    for (int kx = -1; kx <= 1; kx++) {
                        int gray = (data[y + ky][x + kx][0] + 
                                   data[y + ky][x + kx][1] + 
                                   data[y + ky][x + kx][2]) / 3;
                        gx += gray * sobel_x[ky + 1][kx + 1];
                        gy += gray * sobel_y[ky + 1][kx + 1];
                    }
                }
                
                int magnitude = sqrt(gx*gx + gy*gy);
                magnitude = min(255, magnitude);
                result.setPixel(x, y, magnitude, magnitude, magnitude);
            }
        }
        return result;
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
};

class VisualExamples {
public:
    static void createObjectDetectionExample() {
        cout << "Creating object detection example..." << endl;
        
        ImageGenerator scene(400, 300);
        scene.fillBackground(50, 100, 150); // Blue sky background
        
        // Draw ground
        for (int y = 200; y < 300; y++) {
            for (int x = 0; x < 400; x++) {
                scene.setPixel(x, y, 100, 150, 50); // Green ground
            }
        }
        
        // Draw house
        scene.drawRectangle(100, 120, 200, 200, 200, 180, 120, true); // House body
        
        // Draw roof (triangle)
        for (int y = 80; y < 120; y++) {
            int width_offset = (y - 80) * 2;
            int start_x = 150 - width_offset;
            int end_x = 150 + width_offset;
            scene.drawLine(start_x, y, end_x, y, 180, 100, 100); // Red roof
        }
        
        // Draw door
        scene.drawRectangle(130, 160, 150, 200, 120, 80, 40, true);
        
        // Draw windows
        scene.drawRectangle(110, 140, 130, 160, 150, 200, 255, true); // Left window
        scene.drawRectangle(170, 140, 190, 160, 150, 200, 255, true); // Right window
        
        // Draw sun
        scene.drawCircle(350, 50, 25, 255, 255, 100, true);
        
        // Draw tree
        scene.drawRectangle(280, 170, 290, 200, 120, 80, 40, true); // Trunk
        scene.drawCircle(285, 150, 20, 100, 200, 100, true); // Leaves
        
        // Draw detection boxes
        scene.drawRectangle(95, 75, 205, 205, 0, 255, 0, false); // House detection
        scene.drawRectangle(275, 125, 295, 205, 0, 255, 0, false); // Tree detection
        scene.drawRectangle(325, 25, 375, 75, 0, 255, 0, false); // Sun detection
        
        scene.savePPM("object_detection_example.ppm");
        cout << "Saved: object_detection_example.ppm" << endl;
    }

    static void createImageProcessingPipeline() {
        cout << "Creating image processing pipeline..." << endl;
        
        // Create original noisy image
        ImageGenerator original(200, 200);
        original.fillBackground(128, 128, 128);
        
        // Add some objects
        original.drawCircle(50, 50, 20, 255, 255, 255, true);
        original.drawRectangle(120, 120, 180, 180, 200, 200, 200, true);
        original.drawLine(20, 150, 180, 170, 255, 255, 255);
        
        // Add noise
        original.addNoise(30);
        original.savePPM("01_original_noisy.ppm");
        
        // Apply Gaussian blur
        ImageGenerator blurred = original.gaussianBlur();
        blurred.savePPM("02_gaussian_blur.ppm");
        
        // Apply edge detection
        ImageGenerator edges = blurred.edgeDetection();
        edges.savePPM("03_edge_detection.ppm");
        
        cout << "Saved pipeline: 01_original_noisy.ppm -> 02_gaussian_blur.ppm -> 03_edge_detection.ppm" << endl;
    }

    static void createFeatureDetectionExample() {
        cout << "Creating feature detection example..." << endl;
        
        ImageGenerator image(300, 300);
        image.fillBackground(100, 100, 100);
        
        // Create various features to detect
        
        // Corners (L-shapes)
        image.drawLine(50, 50, 100, 50, 255, 255, 255);
        image.drawLine(50, 50, 50, 100, 255, 255, 255);
        
        image.drawLine(250, 50, 200, 50, 255, 255, 255);
        image.drawLine(250, 50, 250, 100, 255, 255, 255);
        
        image.drawLine(50, 250, 100, 250, 255, 255, 255);
        image.drawLine(50, 250, 50, 200, 255, 255, 255);
        
        image.drawLine(250, 250, 200, 250, 255, 255, 255);
        image.drawLine(250, 250, 250, 200, 255, 255, 255);
        
        // Cross (junction)
        image.drawLine(150, 120, 150, 180, 255, 255, 255);
        image.drawLine(120, 150, 180, 150, 255, 255, 255);
        
        // Circles (blobs)
        image.drawCircle(80, 150, 15, 255, 255, 255, false);
        image.drawCircle(220, 150, 12, 255, 255, 255, false);
        
        // Mark detected features with colored dots
        image.drawCircle(50, 50, 5, 255, 0, 0, true);     // Corner detection (red)
        image.drawCircle(250, 50, 5, 255, 0, 0, true);
        image.drawCircle(50, 250, 5, 255, 0, 0, true);
        image.drawCircle(250, 250, 5, 255, 0, 0, true);
        image.drawCircle(150, 150, 5, 255, 0, 0, true);   // Junction detection
        image.drawCircle(80, 150, 5, 0, 255, 0, true);    // Blob detection (green)
        image.drawCircle(220, 150, 5, 0, 255, 0, true);
        
        image.savePPM("feature_detection_example.ppm");
        cout << "Saved: feature_detection_example.ppm (Red=corners, Green=blobs)" << endl;
    }

    static void createMotionDetectionExample() {
        cout << "Creating motion detection example..." << endl;
        
        // Frame 1: Empty room
        ImageGenerator frame1(300, 200);
        frame1.fillBackground(80, 80, 80); // Room background
        
        // Add furniture (static objects)
        frame1.drawRectangle(20, 150, 80, 180, 120, 80, 40, true);  // Table
        frame1.drawRectangle(250, 120, 280, 180, 150, 120, 80, true); // Chair
        
        frame1.savePPM("motion_frame1_empty.ppm");
        
        // Frame 2: Person enters
        ImageGenerator frame2 = frame1; // Copy frame1
        
        // Add person (moving object)
        frame2.drawCircle(150, 80, 15, 220, 180, 140, true);  // Head
        frame2.drawRectangle(135, 95, 165, 150, 100, 150, 200, true); // Body
        frame2.drawRectangle(125, 120, 135, 160, 100, 150, 200, true); // Left arm
        frame2.drawRectangle(165, 120, 175, 160, 100, 150, 200, true); // Right arm
        frame2.drawRectangle(140, 150, 150, 180, 50, 50, 100, true);   // Left leg
        frame2.drawRectangle(150, 150, 160, 180, 50, 50, 100, true);   // Right leg
        
        frame2.savePPM("motion_frame2_person.ppm");
        
        // Create motion difference map
        ImageGenerator motion_diff(300, 200);
        motion_diff.fillBackground(0, 0, 0); // Black background
        
        for (int y = 0; y < 200; y++) {
            for (int x = 0; x < 300; x++) {
                int gray1 = frame1.getPixelGray(x, y);
                int gray2 = frame2.getPixelGray(x, y);
                int diff = abs(gray2 - gray1);
                
                if (diff > 20) { // Motion threshold
                    motion_diff.setPixel(x, y, 255, 255, 255); // White = motion
                }
            }
        }
        
        motion_diff.savePPM("motion_difference_map.ppm");
        
        cout << "Saved motion sequence:" << endl;
        cout << "- motion_frame1_empty.ppm" << endl;
        cout << "- motion_frame2_person.ppm" << endl;
        cout << "- motion_difference_map.ppm (White = motion detected)" << endl;
    }

    static void createMedicalImagingExample() {
        cout << "Creating medical imaging example..." << endl;
        
        ImageGenerator scan(250, 250);
        scan.fillBackground(40, 40, 40); // Dark background
        
        // Create tissue structures
        for (int y = 0; y < 250; y++) {
            for (int x = 0; x < 250; x++) {
                int distance_from_center = sqrt((x-125)*(x-125) + (y-125)*(y-125));
                
                // Create layered tissue appearance
                if (distance_from_center < 100) {
                    int intensity = 60 + (100 - distance_from_center) * 0.8;
                    scan.setPixelGray(x, y, intensity);
                }
            }
        }
        
        // Add organs/structures
        scan.drawCircle(100, 100, 25, 120, 120, 120, true); // Organ 1
        scan.drawCircle(150, 130, 20, 110, 110, 110, true); // Organ 2
        
        // Add potential anomalies
        scan.drawCircle(110, 80, 8, 200, 200, 200, true);   // Bright spot (calcification)
        scan.drawCircle(160, 160, 6, 220, 220, 220, true);  // Another anomaly
        scan.drawCircle(80, 140, 5, 30, 30, 30, true);      // Dark spot (cyst)
        
        // Mark detected anomalies with colored circles
        scan.drawCircle(110, 80, 12, 255, 0, 0, false);     // Red circle around anomaly
        scan.drawCircle(160, 160, 10, 255, 0, 0, false);
        scan.drawCircle(80, 140, 9, 0, 255, 0, false);      // Green circle around cyst
        
        scan.savePPM("medical_scan_analysis.ppm");
        cout << "Saved: medical_scan_analysis.ppm (Red=bright anomalies, Green=dark anomaly)" << endl;
    }

    static void createManufacturingQCExample() {
        cout << "Creating manufacturing quality control example..." << endl;
        
        ImageGenerator product(200, 200);
        product.fillBackground(220, 220, 220); // Light background
        
        // Create product outline (circuit board)
        product.drawRectangle(20, 20, 180, 180, 0, 100, 0, true); // PCB green
        
        // Add good components
        product.drawRectangle(40, 40, 60, 60, 50, 50, 50, true);   // IC1
        product.drawRectangle(120, 40, 140, 60, 50, 50, 50, true); // IC2
        product.drawRectangle(40, 120, 60, 140, 50, 50, 50, true); // IC3
        product.drawRectangle(120, 120, 140, 140, 50, 50, 50, true); // IC4
        
        // Add circuit traces
        product.drawLine(60, 50, 120, 50, 200, 200, 200);
        product.drawLine(50, 60, 50, 120, 200, 200, 200);
        product.drawLine(130, 60, 130, 120, 200, 200, 200);
        product.drawLine(60, 130, 120, 130, 200, 200, 200);
        
        // Add defects
        product.setPixel(70, 70, 255, 0, 0);     // Missing component (red dot)
        product.setPixel(71, 70, 255, 0, 0);
        product.setPixel(70, 71, 255, 0, 0);
        product.setPixel(71, 71, 255, 0, 0);
        
        // Scratch on trace
        product.drawLine(90, 48, 95, 52, 255, 0, 0);
        
        // Wrong component orientation
        product.drawRectangle(125, 125, 135, 135, 255, 0, 0, true);
        
        // Mark defects with detection boxes
        product.drawRectangle(65, 65, 75, 75, 255, 255, 0, false);    // Missing component
        product.drawRectangle(87, 46, 97, 54, 255, 255, 0, false);    // Scratch
        product.drawRectangle(122, 122, 138, 138, 255, 255, 0, false); // Wrong orientation
        
        product.savePPM("manufacturing_qc_example.ppm");
        cout << "Saved: manufacturing_qc_example.ppm (Red=defects, Yellow=detection boxes)" << endl;
    }

    static void createComparisonChart() {
        cout << "Creating algorithm comparison chart..." << endl;
        
        ImageGenerator chart(500, 400);
        chart.fillBackground(255, 255, 255); // White background
        
        // Draw title area
        chart.drawRectangle(0, 0, 499, 50, 50, 50, 100, true);
        
        // Draw sections for different algorithms
        int section_width = 120;
        int section_height = 80;
        
        // Gaussian Blur section
        chart.drawRectangle(20, 70, 20 + section_width, 70 + section_height, 200, 200, 200, true);
        chart.drawCircle(80, 110, 15, 100, 100, 100, true); // Blurred circle
        
        // Edge Detection section  
        chart.drawRectangle(160, 70, 160 + section_width, 70 + section_height, 200, 200, 200, true);
        chart.drawRectangle(200, 90, 240, 130, 0, 0, 0, false); // Edge outline
        
        // Corner Detection section
        chart.drawRectangle(300, 70, 300 + section_width, 70 + section_height, 200, 200, 200, true);
        chart.drawLine(320, 90, 350, 90, 0, 0, 0); // Corner L-shape
        chart.drawLine(320, 90, 320, 120, 0, 0, 0);
        chart.drawCircle(320, 90, 3, 255, 0, 0, true); // Corner point
        
        // Template Matching section
        chart.drawRectangle(20, 180, 20 + section_width, 180 + section_height, 200, 200, 200, true);
        chart.drawRectangle(40, 200, 60, 220, 100, 100, 100, true); // Template
        chart.drawRectangle(80, 210, 100, 230, 100, 100, 100, true); // Match
        chart.drawRectangle(78, 208, 102, 232, 0, 255, 0, false); // Match box
        
        // Motion Detection section
        chart.drawRectangle(160, 180, 160 + section_width, 180 + section_height, 200, 200, 200, true);
        chart.drawCircle(200, 220, 8, 255, 255, 255, true); // Moving object
        chart.drawLine(190, 220, 210, 220, 255, 0, 0); // Motion vector
        
        // Object Recognition section
        chart.drawRectangle(300, 180, 300 + section_width, 180 + section_height, 200, 200, 200, true);
        chart.drawCircle(340, 200, 8, 255, 255, 100, true); // Face
        chart.setPixel(335, 195, 0, 0, 0); // Left eye
        chart.setPixel(345, 195, 0, 0, 0); // Right eye
        chart.drawLine(335, 205, 345, 205, 0, 0, 0); // Mouth
        chart.drawRectangle(330, 190, 350, 210, 0, 255, 0, false); // Detection box
        
        // Performance comparison bars
        chart.drawRectangle(50, 300, 70, 350, 100, 255, 100, true);  // Fast
        chart.drawRectangle(120, 320, 140, 350, 255, 255, 100, true); // Medium
        chart.drawRectangle(190, 330, 210, 350, 255, 100, 100, true); // Slow
        chart.drawRectangle(260, 310, 280, 350, 100, 255, 100, true); // Fast
        chart.drawRectangle(330, 325, 350, 350, 255, 255, 100, true); // Medium
        chart.drawRectangle(400, 340, 420, 350, 255, 100, 100, true); // Very slow
        
        chart.savePPM("algorithm_comparison_chart.ppm");
        cout << "Saved: algorithm_comparison_chart.ppm" << endl;
        cout << "Chart shows: Blur, Edge Detection, Corner Detection, Template Matching, Motion Detection, Object Recognition" << endl;
        cout << "Bottom bars show relative processing speeds (Green=Fast, Yellow=Medium, Red=Slow)" << endl;
    }
};

int main() {
    cout << "🎨 VISUAL COMPUTER VISION EXAMPLES GENERATOR" << endl;
    cout << "=============================================" << endl;
    cout << "Creating visual examples as image files..." << endl << endl;
    
    try {
        // Create various visual examples
        VisualExamples::createObjectDetectionExample();
        VisualExamples::createImageProcessingPipeline();
        VisualExamples::createFeatureDetectionExample();
        VisualExamples::createMotionDetectionExample();
        VisualExamples::createMedicalImagingExample();
        VisualExamples::createManufacturingQCExample();
        VisualExamples::createComparisonChart();
        
        cout << endl << "✅ ALL VISUAL EXAMPLES CREATED!" << endl;
        cout << "Generated image files (PPM format):" << endl;
        cout << "📸 object_detection_example.ppm - Scene with detected objects" << endl;
        cout << "📸 01_original_noisy.ppm - Original noisy image" << endl;
        cout << "📸 02_gaussian_blur.ppm - After noise reduction" << endl;
        cout << "📸 03_edge_detection.ppm - After edge detection" << endl;
        cout << "📸 feature_detection_example.ppm - Corner and blob detection" << endl;
        cout << "📸 motion_frame1_empty.ppm - Security camera frame 1" << endl;
        cout << "📸 motion_frame2_person.ppm - Security camera frame 2" << endl;
        cout << "📸 motion_difference_map.ppm - Motion detection result" << endl;
        cout << "📸 medical_scan_analysis.ppm - Medical imaging with anomalies" << endl;
        cout << "📸 manufacturing_qc_example.ppm - Quality control inspection" << endl;
        cout << "📸 algorithm_comparison_chart.ppm - Algorithm overview" << endl;
        
        cout << endl << "🖼️  HOW TO VIEW THE IMAGES:" << endl;
        cout << "Method 1: Use an image viewer that supports PPM format" << endl;
        cout << "Method 2: Convert to common formats:" << endl;
        cout << "  - On Linux: convert file.ppm file.png" << endl;
        cout << "  - Online: Upload PPM files to online converters" << endl;
        cout << "Method 3: Open with text editor to see raw RGB data" << endl;
        
        cout << endl << "🎯 WHAT EACH IMAGE DEMONSTRATES:" << endl;
        cout << "• Object Detection: How algorithms identify and locate objects" << endl;
        cout << "• Image Processing: Step-by-step noise reduction and edge detection" << endl;
        cout << "• Feature Detection: Corner and blob detection with colored markers" << endl;
        cout << "• Motion Detection: Security system detecting person movement" << endl;
        cout << "• Medical Imaging: Anomaly detection in medical scans" << endl;
        cout << "• Quality Control: Defect detection in manufacturing" << endl;
        cout << "• Algorithm Overview: Visual comparison of different CV techniques" << endl;
        
        return 0;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
}