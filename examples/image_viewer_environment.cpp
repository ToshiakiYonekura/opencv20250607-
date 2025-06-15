/**
 * @file image_viewer_environment.cpp
 * @brief Complete Image Viewing Environment
 * 
 * This creates multiple ways to view and analyze the generated computer vision examples:
 * - ASCII art visualization in terminal
 * - HTML gallery generator
 * - Image analysis and statistics
 * - Interactive image browser
 */

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <map>

using namespace std;

struct PixelRGB {
    int r, g, b;
    PixelRGB(int red = 0, int green = 0, int blue = 0) : r(red), g(green), b(blue) {}
};

class PPMImage {
private:
    vector<vector<PixelRGB>> pixels;
    int width, height;
    string filename;

public:
    PPMImage(const string& file) : filename(file), width(0), height(0) {
        loadPPM(file);
    }

    bool loadPPM(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Cannot open " << filename << endl;
            return false;
        }

        string magic;
        file >> magic;
        if (magic != "P3") {
            cout << "Error: Not a P3 PPM file" << endl;
            return false;
        }

        file >> width >> height;
        int maxval;
        file >> maxval;

        pixels.resize(height, vector<PixelRGB>(width));

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                file >> pixels[y][x].r >> pixels[y][x].g >> pixels[y][x].b;
            }
        }

        file.close();
        return true;
    }

    // Convert to ASCII art
    void displayASCII(int scale = 4) const {
        cout << "\n" << filename << " (" << width << "x" << height << ")" << endl;
        cout << string(width/scale + 2, '=') << endl;

        for (int y = 0; y < height; y += scale) {
            for (int x = 0; x < width; x += scale) {
                int gray = (pixels[y][x].r + pixels[y][x].g + pixels[y][x].b) / 3;
                char c = ' ';
                if (gray > 200) c = '#';
                else if (gray > 150) c = '*';
                else if (gray > 100) c = '+';
                else if (gray > 50) c = '-';
                else if (gray > 25) c = '.';
                else c = ' ';
                cout << c;
            }
            cout << endl;
        }
        cout << string(width/scale + 2, '=') << endl;
    }

    // Show color-coded ASCII
    void displayColorASCII(int scale = 4) const {
        cout << "\n" << filename << " - Color Analysis (" << width << "x" << height << ")" << endl;
        cout << string(width/scale + 2, '=') << endl;

        for (int y = 0; y < height; y += scale) {
            for (int x = 0; x < width; x += scale) {
                const PixelRGB& p = pixels[y][x];
                char c;
                
                // Classify dominant color
                if (p.r > p.g && p.r > p.b && p.r > 100) c = 'R'; // Red
                else if (p.g > p.r && p.g > p.b && p.g > 100) c = 'G'; // Green  
                else if (p.b > p.r && p.b > p.g && p.b > 100) c = 'B'; // Blue
                else if (p.r > 150 && p.g > 150 && p.b > 150) c = 'W'; // White
                else if (p.r < 50 && p.g < 50 && p.b < 50) c = ' '; // Black
                else if (p.r > 100 && p.g > 100 && p.b < 50) c = 'Y'; // Yellow
                else c = '.'; // Gray
                
                cout << c;
            }
            cout << endl;
        }
        cout << string(width/scale + 2, '=') << endl;
        cout << "Legend: R=Red, G=Green, B=Blue, W=White, Y=Yellow, .=Gray, [space]=Black" << endl;
    }

    // Analyze image statistics
    void analyzeImage() const {
        map<string, int> colorCounts;
        int totalPixels = width * height;
        long long rSum = 0, gSum = 0, bSum = 0;
        int minR = 255, maxR = 0, minG = 255, maxG = 0, minB = 255, maxB = 0;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                const PixelRGB& p = pixels[y][x];
                
                rSum += p.r; gSum += p.g; bSum += p.b;
                minR = min(minR, p.r); maxR = max(maxR, p.r);
                minG = min(minG, p.g); maxG = max(maxG, p.g);
                minB = min(minB, p.b); maxB = max(maxB, p.b);

                // Classify colors
                if (p.r > 150 && p.g < 100 && p.b < 100) colorCounts["Red"]++;
                else if (p.g > 150 && p.r < 100 && p.b < 100) colorCounts["Green"]++;
                else if (p.b > 150 && p.r < 100 && p.g < 100) colorCounts["Blue"]++;
                else if (p.r > 150 && p.g > 150 && p.b > 150) colorCounts["White"]++;
                else if (p.r < 50 && p.g < 50 && p.b < 50) colorCounts["Black"]++;
                else if (p.r > 150 && p.g > 150 && p.b < 100) colorCounts["Yellow"]++;
                else colorCounts["Other"]++;
            }
        }

        cout << "\n📊 IMAGE ANALYSIS: " << filename << endl;
        cout << "===========================================" << endl;
        cout << "Dimensions: " << width << " x " << height << " pixels" << endl;
        cout << "Total pixels: " << totalPixels << endl;
        cout << "\nColor Averages:" << endl;
        cout << "  Red:   " << (rSum / totalPixels) << " (range: " << minR << "-" << maxR << ")" << endl;
        cout << "  Green: " << (gSum / totalPixels) << " (range: " << minG << "-" << maxG << ")" << endl;
        cout << "  Blue:  " << (bSum / totalPixels) << " (range: " << minB << "-" << maxB << ")" << endl;
        
        cout << "\nColor Distribution:" << endl;
        for (const auto& pair : colorCounts) {
            double percentage = (double)pair.second / totalPixels * 100;
            cout << "  " << pair.first << ": " << pair.second << " pixels (" 
                 << fixed << setprecision(1) << percentage << "%)" << endl;
        }
    }

    // Find interesting regions
    void findFeatures() const {
        cout << "\n🔍 FEATURE DETECTION: " << filename << endl;
        cout << "===========================================" << endl;

        // Find bright spots (potential detections/highlights)
        vector<pair<int, int>> brightSpots;
        for (int y = 1; y < height - 1; y++) {
            for (int x = 1; x < width - 1; x++) {
                const PixelRGB& p = pixels[y][x];
                int brightness = (p.r + p.g + p.b) / 3;
                
                if (brightness > 200) {
                    // Check if it's brighter than surroundings
                    bool isBrightest = true;
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            if (dx == 0 && dy == 0) continue;
                            const PixelRGB& neighbor = pixels[y + dy][x + dx];
                            int neighborBrightness = (neighbor.r + neighbor.g + neighbor.b) / 3;
                            if (neighborBrightness >= brightness) {
                                isBrightest = false;
                                break;
                            }
                        }
                        if (!isBrightest) break;
                    }
                    if (isBrightest) {
                        brightSpots.push_back({x, y});
                    }
                }
            }
        }

        cout << "Bright spots found: " << brightSpots.size() << endl;
        for (size_t i = 0; i < min(brightSpots.size(), size_t(10)); i++) {
            cout << "  Spot " << i+1 << ": (" << brightSpots[i].first << ", " << brightSpots[i].second << ")" << endl;
        }

        // Find color regions
        map<string, vector<pair<int, int>>> colorRegions;
        for (int y = 0; y < height; y += 10) {
            for (int x = 0; x < width; x += 10) {
                const PixelRGB& p = pixels[y][x];
                if (p.r > 200 && p.g < 100 && p.b < 100) colorRegions["Red"].push_back({x, y});
                if (p.g > 200 && p.r < 100 && p.b < 100) colorRegions["Green"].push_back({x, y});
                if (p.b > 200 && p.r < 100 && p.g < 100) colorRegions["Blue"].push_back({x, y});
            }
        }

        cout << "\nColor regions detected:" << endl;
        for (const auto& pair : colorRegions) {
            if (pair.second.size() > 0) {
                cout << "  " << pair.first << " regions: " << pair.second.size() << endl;
            }
        }
    }

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    string getFilename() const { return filename; }
};

class ImageViewerEnvironment {
public:
    static void createHTMLGallery(const vector<string>& imageFiles) {
        ofstream html("image_gallery.html");
        
        html << "<!DOCTYPE html>\n<html>\n<head>\n";
        html << "<title>Computer Vision Examples Gallery</title>\n";
        html << "<style>\n";
        html << "body { font-family: Arial, sans-serif; margin: 20px; background: #f5f5f5; }\n";
        html << ".gallery { display: grid; grid-template-columns: repeat(auto-fit, minmax(400px, 1fr)); gap: 20px; }\n";
        html << ".image-card { background: white; border-radius: 8px; padding: 15px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }\n";
        html << ".image-title { font-size: 18px; font-weight: bold; margin-bottom: 10px; color: #333; }\n";
        html << ".image-desc { color: #666; margin-bottom: 15px; }\n";
        html << ".image-placeholder { width: 100%; height: 200px; background: #e0e0e0; border-radius: 4px; }\n";
        html << ".image-placeholder { display: flex; align-items: center; justify-content: center; color: #888; }\n";
        html << ".stats { background: #f8f9fa; padding: 10px; border-radius: 4px; margin-top: 10px; font-size: 14px; }\n";
        html << "h1 { color: #2c3e50; text-align: center; }\n";
        html << ".header { text-align: center; margin-bottom: 30px; }\n";
        html << ".note { background: #e8f4fd; border: 1px solid #bee5eb; padding: 15px; border-radius: 4px; margin-bottom: 20px; }\n";
        html << "</style>\n</head>\n<body>\n";
        
        html << "<div class='header'>\n";
        html << "<h1>🎨 Computer Vision Examples Gallery</h1>\n";
        html << "<p>Visual demonstrations of computer vision algorithms and real-world applications</p>\n";
        html << "</div>\n";
        
        html << "<div class='note'>\n";
        html << "<strong>Note:</strong> Images are in PPM format. To view them properly:\n";
        html << "<ul>\n";
        html << "<li>Convert to PNG: <code>convert file.ppm file.png</code> (requires ImageMagick)</li>\n";
        html << "<li>Use online converter: photopea.com or convertio.co</li>\n";
        html << "<li>View with GIMP, IrfanView, or modern browsers</li>\n";
        html << "</ul>\n";
        html << "</div>\n";
        
        html << "<div class='gallery'>\n";
        
        // Define descriptions for each image
        map<string, pair<string, string>> imageInfo = {
            {"object_detection_example.ppm", {"🏠 Object Detection Demo", "Scene with house, tree, and sun. Green boxes show detected objects using computer vision algorithms."}},
            {"01_original_noisy.ppm", {"📸 Original Noisy Image", "Starting image with geometric shapes and added noise, simulating real-world image capture conditions."}},
            {"02_gaussian_blur.ppm", {"🔧 Gaussian Blur Applied", "Noise reduction using Gaussian filter. Notice how the image becomes smoother while preserving major features."}},
            {"03_edge_detection.ppm", {"🔍 Edge Detection Result", "Sobel edge detection applied after noise reduction. White areas show detected boundaries and edges."}},
            {"feature_detection_example.ppm", {"🎯 Feature Detection", "Corner detection (red dots) and blob detection (green dots) on various geometric patterns."}},
            {"motion_frame1_empty.ppm", {"🎥 Security Camera - Frame 1", "Empty room surveillance footage. This serves as the background reference for motion detection."}},
            {"motion_frame2_person.ppm", {"🎥 Security Camera - Frame 2", "Same room with person present. Blue figure represents detected human movement."}},
            {"motion_difference_map.ppm", {"🚨 Motion Detection Result", "White pixels indicate areas where motion was detected between the two frames."}},
            {"medical_scan_analysis.ppm", {"🏥 Medical Imaging Analysis", "Simulated medical scan with anomaly detection. Red circles highlight potential areas of concern."}},
            {"manufacturing_qc_example.ppm", {"🏭 Quality Control Inspection", "Circuit board inspection with defect detection. Yellow boxes mark detected manufacturing defects."}},
            {"algorithm_comparison_chart.ppm", {"📊 Algorithm Comparison", "Visual overview of different computer vision techniques with performance indicators."}}
        };
        
        for (const string& filename : imageFiles) {
            auto info = imageInfo.find(filename);
            string title = (info != imageInfo.end()) ? info->second.first : filename;
            string desc = (info != imageInfo.end()) ? info->second.second : "Computer vision demonstration image";
            
            html << "<div class='image-card'>\n";
            html << "<div class='image-title'>" << title << "</div>\n";
            html << "<div class='image-desc'>" << desc << "</div>\n";
            html << "<div class='image-placeholder'>\n";
            html << filename << "<br>Click to download and view\n";
            html << "</div>\n";
            html << "<div class='stats'>\n";
            html << "<strong>File:</strong> " << filename << "<br>\n";
            html << "<strong>Format:</strong> PPM (Portable Pixmap)<br>\n";
            html << "<strong>Purpose:</strong> " << desc << "\n";
            html << "</div>\n";
            html << "</div>\n";
        }
        
        html << "</div>\n";
        
        html << "<div style='margin-top: 40px; text-align: center; color: #666;'>\n";
        html << "<p>Generated by Computer Vision Examples Generator</p>\n";
        html << "<p>These images demonstrate fundamental computer vision concepts used in industry applications.</p>\n";
        html << "</div>\n";
        
        html << "</body>\n</html>";
        html.close();
        
        cout << "✅ Created HTML gallery: image_gallery.html" << endl;
    }

    static void createImageIndex(const vector<string>& imageFiles) {
        ofstream index("image_index.txt");
        
        index << "COMPUTER VISION EXAMPLES - IMAGE INDEX\n";
        index << "======================================\n\n";
        
        index << "Location: " << "/home/yonetoshi2088/work/opencv20250606-/opencv20250607-/examples/\n\n";
        
        for (size_t i = 0; i < imageFiles.size(); i++) {
            index << (i + 1) << ". " << imageFiles[i] << "\n";
            
            // Add descriptions
            if (imageFiles[i].find("object_detection") != string::npos) {
                index << "   → Object detection with green bounding boxes\n";
            } else if (imageFiles[i].find("01_original") != string::npos) {
                index << "   → Original noisy image (processing pipeline step 1)\n";
            } else if (imageFiles[i].find("02_gaussian") != string::npos) {
                index << "   → Gaussian blur applied (processing pipeline step 2)\n";
            } else if (imageFiles[i].find("03_edge") != string::npos) {
                index << "   → Edge detection result (processing pipeline step 3)\n";
            } else if (imageFiles[i].find("feature_detection") != string::npos) {
                index << "   → Feature detection: red=corners, green=blobs\n";
            } else if (imageFiles[i].find("motion_frame1") != string::npos) {
                index << "   → Security camera: empty room (motion detection baseline)\n";
            } else if (imageFiles[i].find("motion_frame2") != string::npos) {
                index << "   → Security camera: person detected\n";
            } else if (imageFiles[i].find("motion_difference") != string::npos) {
                index << "   → Motion detection result: white=movement\n";
            } else if (imageFiles[i].find("medical") != string::npos) {
                index << "   → Medical scan with anomaly detection (red circles)\n";
            } else if (imageFiles[i].find("manufacturing") != string::npos) {
                index << "   → Quality control: circuit board defect detection\n";
            } else if (imageFiles[i].find("algorithm") != string::npos) {
                index << "   → Algorithm comparison chart with performance bars\n";
            }
            index << "\n";
        }
        
        index << "\nVIEWING OPTIONS:\n";
        index << "================\n";
        index << "1. ASCII Art View: ./image_viewer_environment ascii <filename>\n";
        index << "2. Color Analysis: ./image_viewer_environment color <filename>\n";
        index << "3. Statistics: ./image_viewer_environment analyze <filename>\n";
        index << "4. Feature Detection: ./image_viewer_environment features <filename>\n";
        index << "5. HTML Gallery: ./image_viewer_environment gallery\n";
        index << "6. Convert to PNG: convert <filename.ppm> <filename.png>\n";
        index << "7. Online Viewer: Upload to photopea.com or convertio.co\n";
        
        index.close();
        cout << "✅ Created image index: image_index.txt" << endl;
    }

    static void createViewerScript() {
        ofstream script("view_images.sh");
        
        script << "#!/bin/bash\n";
        script << "# Computer Vision Examples Viewer Script\n\n";
        
        script << "echo \"🎨 Computer Vision Examples Viewer\"\n";
        script << "echo \"==================================\"\n";
        script << "echo \"Available images:\"\n";
        script << "ls -1 *.ppm | nl\n";
        script << "echo \"\"\n";
        
        script << "if [ \"$1\" == \"\" ]; then\n";
        script << "    echo \"Usage: $0 [command] [filename]\"\n";
        script << "    echo \"Commands:\"\n";
        script << "    echo \"  list     - List all images\"\n";
        script << "    echo \"  ascii    - View as ASCII art\"\n";
        script << "    echo \"  color    - View with color coding\"\n";
        script << "    echo \"  analyze  - Show image statistics\"\n";
        script << "    echo \"  features - Detect features\"\n";
        script << "    echo \"  gallery  - Create HTML gallery\"\n";
        script << "    echo \"  convert  - Convert PPM to PNG (requires ImageMagick)\"\n";
        script << "    exit 1\n";
        script << "fi\n\n";
        
        script << "case $1 in\n";
        script << "    list)\n";
        script << "        echo \"Available images:\"\n";
        script << "        ls -la *.ppm\n";
        script << "        ;;\n";
        script << "    ascii)\n";
        script << "        if [ \"$2\" != \"\" ]; then\n";
        script << "            ./image_viewer_environment ascii \"$2\"\n";
        script << "        else\n";
        script << "            echo \"Usage: $0 ascii <filename.ppm>\"\n";
        script << "        fi\n";
        script << "        ;;\n";
        script << "    color)\n";
        script << "        if [ \"$2\" != \"\" ]; then\n";
        script << "            ./image_viewer_environment color \"$2\"\n";
        script << "        else\n";
        script << "            echo \"Usage: $0 color <filename.ppm>\"\n";
        script << "        fi\n";
        script << "        ;;\n";
        script << "    convert)\n";
        script << "        if command -v convert &> /dev/null; then\n";
        script << "            for file in *.ppm; do\n";
        script << "                echo \"Converting $file to ${file%.ppm}.png\"\n";
        script << "                convert \"$file\" \"${file%.ppm}.png\"\n";
        script << "            done\n";
        script << "            echo \"✅ Conversion complete!\"\n";
        script << "        else\n";
        script << "            echo \"❌ ImageMagick not installed. Please install: sudo apt install imagemagick\"\n";
        script << "        fi\n";
        script << "        ;;\n";
        script << "    gallery)\n";
        script << "        ./image_viewer_environment gallery\n";
        script << "        echo \"✅ Open image_gallery.html in your browser\"\n";
        script << "        ;;\n";
        script << "    *)\n";
        script << "        echo \"Unknown command: $1\"\n";
        script << "        ;;\n";
        script << "esac\n";
        
        script.close();
        cout << "✅ Created viewer script: view_images.sh" << endl;
    }
};

int main(int argc, char* argv[]) {
    cout << "🖼️  COMPUTER VISION IMAGE VIEWING ENVIRONMENT" << endl;
    cout << "=============================================" << endl;
    
    vector<string> imageFiles = {
        "object_detection_example.ppm",
        "01_original_noisy.ppm", 
        "02_gaussian_blur.ppm",
        "03_edge_detection.ppm",
        "feature_detection_example.ppm",
        "motion_frame1_empty.ppm",
        "motion_frame2_person.ppm", 
        "motion_difference_map.ppm",
        "medical_scan_analysis.ppm",
        "manufacturing_qc_example.ppm",
        "algorithm_comparison_chart.ppm"
    };
    
    if (argc < 2) {
        cout << "\nCreating viewing environment for computer vision examples...\n" << endl;
        
        // Create environment files
        ImageViewerEnvironment::createHTMLGallery(imageFiles);
        ImageViewerEnvironment::createImageIndex(imageFiles);
        ImageViewerEnvironment::createViewerScript();
        
        cout << "\n🎯 ENVIRONMENT CREATED! Available tools:" << endl;
        cout << "1. HTML Gallery: open image_gallery.html in browser" << endl;
        cout << "2. Image Index: cat image_index.txt" << endl;
        cout << "3. Viewer Script: chmod +x view_images.sh && ./view_images.sh" << endl;
        cout << "4. Direct viewing: " << argv[0] << " [command] [filename]" << endl;
        
        cout << "\n📋 Available commands:" << endl;
        cout << "- ascii <file>    : View as ASCII art" << endl;
        cout << "- color <file>    : View with color analysis" << endl;
        cout << "- analyze <file>  : Show detailed statistics" << endl;
        cout << "- features <file> : Detect interesting features" << endl;
        cout << "- gallery         : Create HTML gallery" << endl;
        
        return 0;
    }
    
    string command = argv[1];
    
    if (command == "gallery") {
        ImageViewerEnvironment::createHTMLGallery(imageFiles);
        return 0;
    }
    
    if (argc < 3) {
        cout << "Error: Please specify a filename" << endl;
        cout << "Usage: " << argv[0] << " " << command << " <filename.ppm>" << endl;
        return 1;
    }
    
    string filename = argv[2];
    PPMImage image(filename);
    
    if (image.getWidth() == 0) {
        cout << "Error: Could not load image " << filename << endl;
        return 1;
    }
    
    if (command == "ascii") {
        image.displayASCII();
    } else if (command == "color") {
        image.displayColorASCII();
    } else if (command == "analyze") {
        image.analyzeImage();
    } else if (command == "features") {
        image.findFeatures();
    } else {
        cout << "Unknown command: " << command << endl;
        cout << "Available commands: ascii, color, analyze, features, gallery" << endl;
        return 1;
    }
    
    return 0;
}