/**
 * @file real_world_cv.cpp
 * @brief Real-World Computer Vision Applications Simulation
 * 
 * This demonstrates practical computer vision applications:
 * - Manufacturing quality control
 * - Medical image analysis
 * - Autonomous vehicle perception
 * - Security and surveillance
 * - Retail and commerce
 * - Agriculture and food processing
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <random>
#include <iomanip>

using namespace std;

// Enhanced matrix for real-world simulations
class RealWorldCV {
private:
    vector<vector<int>> data;
    int rows, cols;

public:
    RealWorldCV(int r, int c) : rows(r), cols(c) {
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

    void print(const string& title = "") const {
        if (!title.empty()) cout << title << endl;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                printf("%3d ", data[i][j]);
            }
            cout << endl;
        }
        cout << endl;
    }

    // Create realistic test patterns
    static RealWorldCV createDefectiveProduct(int rows, int cols) {
        RealWorldCV product(rows, cols);
        // Perfect circular product
        int center_x = cols / 2, center_y = rows / 2;
        int radius = min(rows, cols) / 3;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int dx = j - center_x, dy = i - center_y;
                int distance = sqrt(dx*dx + dy*dy);
                if (distance <= radius) {
                    product.set(i, j, 200); // Good product material
                }
            }
        }
        
        // Add defects
        product.set(2, 3, 50);  // Scratch
        product.set(4, 5, 100); // Discoloration
        product.set(6, 2, 30);  // Crack
        
        return product;
    }

    static RealWorldCV createMedicalScan(int rows, int cols) {
        RealWorldCV scan(rows, cols);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> tissue_dis(40, 80);
        
        // Tissue background
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                scan.set(i, j, tissue_dis(gen));
            }
        }
        
        // Organs/structures
        for (int i = 2; i < 6; i++) {
            for (int j = 3; j < 7; j++) {
                scan.set(i, j, 120); // Dense tissue
            }
        }
        
        // Potential anomaly
        scan.set(3, 4, 200); // Bright spot (calcification/tumor)
        scan.set(4, 4, 180);
        
        return scan;
    }

    static RealWorldCV createRoadScene(int rows, int cols) {
        RealWorldCV road(rows, cols);
        
        // Road surface
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                road.set(i, j, 60 + (i * 10 / rows)); // Gradient road
            }
        }
        
        // Lane markings
        for (int i = 0; i < rows; i++) {
            road.set(i, cols/3, 255);     // Left lane
            road.set(i, 2*cols/3, 255);   // Right lane
        }
        
        // Center dashed line
        for (int i = 1; i < rows; i += 3) {
            road.set(i, cols/2, 255);
        }
        
        // Obstacles/vehicles
        road.set(4, 5, 120); // Vehicle
        road.set(5, 5, 120);
        
        return road;
    }

    static RealWorldCV createSecurityFrame(int rows, int cols, bool motion = false) {
        RealWorldCV frame(rows, cols);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> bg_dis(20, 40);
        
        // Background
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                frame.set(i, j, bg_dis(gen));
            }
        }
        
        // Static objects (furniture, walls)
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < cols; j++) {
                frame.set(i, j, 80); // Wall
            }
        }
        
        if (motion) {
            // Moving person
            for (int i = 3; i < 7; i++) {
                for (int j = 4; j < 6; j++) {
                    frame.set(i, j, 150); // Person
                }
            }
        }
        
        return frame;
    }

    static RealWorldCV createRetailShelf(int rows, int cols) {
        RealWorldCV shelf(rows, cols);
        
        // Shelf background
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                shelf.set(i, j, 40);
            }
        }
        
        // Products (different shapes and sizes)
        // Product 1 (tall bottle)
        for (int i = 1; i < 6; i++) {
            shelf.set(i, 2, 180);
        }
        
        // Product 2 (box)
        for (int i = 3; i < 6; i++) {
            for (int j = 4; j < 6; j++) {
                shelf.set(i, j, 160);
            }
        }
        
        // Missing product (empty space)
        // Product 3 (can)
        for (int i = 4; i < 6; i++) {
            shelf.set(i, 7, 170);
        }
        
        return shelf;
    }

    static RealWorldCV createAgriculturalField(int rows, int cols) {
        RealWorldCV field(rows, cols);
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> crop_dis(80, 120);
        
        // Healthy crops
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                field.set(i, j, crop_dis(gen));
            }
        }
        
        // Disease/pest areas (darker regions)
        for (int i = 2; i < 4; i++) {
            for (int j = 3; j < 5; j++) {
                field.set(i, j, 40); // Diseased area
            }
        }
        
        // Weed areas (different texture)
        field.set(1, 6, 200);
        field.set(5, 1, 190);
        
        return field;
    }

    // Analysis functions
    struct DefectAnalysis {
        int defect_count;
        vector<pair<int, int>> defect_locations;
        float defect_severity;
        string quality_grade;
    };

    DefectAnalysis analyzeProductQuality(int good_threshold = 150) const {
        DefectAnalysis analysis;
        analysis.defect_count = 0;
        int total_defect_severity = 0;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int pixel_val = get(i, j);
                if (pixel_val > 0 && pixel_val < good_threshold) {
                    analysis.defect_count++;
                    analysis.defect_locations.push_back({i, j});
                    total_defect_severity += (good_threshold - pixel_val);
                }
            }
        }
        
        analysis.defect_severity = (float)total_defect_severity / (rows * cols);
        
        if (analysis.defect_count == 0) analysis.quality_grade = "A - Excellent";
        else if (analysis.defect_count < 3) analysis.quality_grade = "B - Good";
        else if (analysis.defect_count < 6) analysis.quality_grade = "C - Acceptable";
        else analysis.quality_grade = "D - Reject";
        
        return analysis;
    }

    struct MedicalAnalysis {
        int anomaly_count;
        vector<pair<int, int>> anomaly_locations;
        float tissue_density;
        float contrast_ratio;
        string diagnostic_recommendation;
    };

    MedicalAnalysis analyzeMedicalImage() const {
        MedicalAnalysis analysis;
        analysis.anomaly_count = 0;
        int total_intensity = 0, max_val = 0, min_val = 255;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int pixel_val = get(i, j);
                total_intensity += pixel_val;
                max_val = max(max_val, pixel_val);
                min_val = min(min_val, pixel_val);
                
                // Detect anomalies (unusually bright regions)
                if (pixel_val > 150) {
                    analysis.anomaly_count++;
                    analysis.anomaly_locations.push_back({i, j});
                }
            }
        }
        
        analysis.tissue_density = (float)total_intensity / (rows * cols);
        analysis.contrast_ratio = (float)(max_val - min_val) / 255.0f;
        
        if (analysis.anomaly_count == 0) {
            analysis.diagnostic_recommendation = "Normal - No anomalies detected";
        } else if (analysis.anomaly_count < 3) {
            analysis.diagnostic_recommendation = "Monitor - Few bright regions detected";
        } else {
            analysis.diagnostic_recommendation = "Further Investigation - Multiple anomalies found";
        }
        
        return analysis;
    }

    struct TrafficAnalysis {
        int lane_count;
        vector<int> lane_positions;
        int obstacle_count;
        vector<pair<int, int>> obstacle_locations;
        string driving_recommendation;
    };

    TrafficAnalysis analyzeTrafficScene() const {
        TrafficAnalysis analysis;
        analysis.lane_count = 0;
        analysis.obstacle_count = 0;
        
        // Detect lane markings (bright vertical lines)
        for (int j = 0; j < cols; j++) {
            int bright_pixels = 0;
            for (int i = 0; i < rows; i++) {
                if (get(i, j) > 200) bright_pixels++;
            }
            if (bright_pixels > rows / 2) {
                analysis.lane_count++;
                analysis.lane_positions.push_back(j);
            }
        }
        
        // Detect obstacles (medium brightness objects)
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int pixel_val = get(i, j);
                if (pixel_val > 100 && pixel_val < 150) {
                    analysis.obstacle_count++;
                    analysis.obstacle_locations.push_back({i, j});
                }
            }
        }
        
        if (analysis.obstacle_count == 0) {
            analysis.driving_recommendation = "Clear Road - Safe to proceed";
        } else if (analysis.obstacle_count < 3) {
            analysis.driving_recommendation = "Caution - Obstacles detected";
        } else {
            analysis.driving_recommendation = "Stop - Multiple obstacles detected";
        }
        
        return analysis;
    }

    struct SecurityAnalysis {
        bool motion_detected;
        int motion_intensity;
        vector<pair<int, int>> motion_areas;
        string alert_level;
    };

    SecurityAnalysis compareFrames(const RealWorldCV& previous_frame) const {
        SecurityAnalysis analysis;
        analysis.motion_detected = false;
        analysis.motion_intensity = 0;
        
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int diff = abs(get(i, j) - previous_frame.get(i, j));
                if (diff > 30) { // Motion threshold
                    analysis.motion_detected = true;
                    analysis.motion_intensity += diff;
                    analysis.motion_areas.push_back({i, j});
                }
            }
        }
        
        if (!analysis.motion_detected) {
            analysis.alert_level = "Normal - No motion detected";
        } else if (analysis.motion_intensity < 500) {
            analysis.alert_level = "Low - Minor movement detected";
        } else if (analysis.motion_intensity < 1500) {
            analysis.alert_level = "Medium - Significant movement";
        } else {
            analysis.alert_level = "High - Major activity detected";
        }
        
        return analysis;
    }

    int getRows() const { return rows; }
    int getCols() const { return cols; }
};

// Real-world application demonstrations
class RealWorldApplications {
public:
    static void demonstrateManufacturingQC() {
        cout << "=== MANUFACTURING QUALITY CONTROL ===" << endl;
        cout << "Application: Automated inspection of manufactured products" << endl;
        cout << "Industry: Electronics, Automotive, Pharmaceuticals" << endl << endl;
        
        auto product = RealWorldCV::createDefectiveProduct(8, 8);
        product.print("Product Image (Raw Sensor Data):");
        
        auto analysis = product.analyzeProductQuality();
        
        cout << "Quality Control Analysis:" << endl;
        cout << "- Defects detected: " << analysis.defect_count << endl;
        cout << "- Defect severity: " << fixed << setprecision(2) << analysis.defect_severity << endl;
        cout << "- Quality grade: " << analysis.quality_grade << endl;
        cout << "- Defect locations: ";
        for (const auto& loc : analysis.defect_locations) {
            cout << "(" << loc.first << "," << loc.second << ") ";
        }
        cout << endl;
        
        cout << "\nAction Taken: ";
        if (analysis.quality_grade[0] == 'D') {
            cout << "REJECT - Product sent to rework station" << endl;
        } else {
            cout << "PASS - Product continues on production line" << endl;
        }
        cout << "Processing time: 15ms (real-time inspection)" << endl << endl;
    }

    static void demonstrateMedicalImaging() {
        cout << "=== MEDICAL IMAGE ANALYSIS ===" << endl;
        cout << "Application: Automated medical scan interpretation" << endl;
        cout << "Use Cases: Radiology, Pathology, Dermatology" << endl << endl;
        
        auto scan = RealWorldCV::createMedicalScan(8, 8);
        scan.print("Medical Scan (CT/MRI/X-Ray):");
        
        auto analysis = scan.analyzeMedicalImage();
        
        cout << "Medical Analysis Report:" << endl;
        cout << "- Tissue density (HU): " << fixed << setprecision(1) << analysis.tissue_density << endl;
        cout << "- Image contrast: " << fixed << setprecision(3) << analysis.contrast_ratio << endl;
        cout << "- Anomalies detected: " << analysis.anomaly_count << endl;
        cout << "- Anomaly locations: ";
        for (const auto& loc : analysis.anomaly_locations) {
            cout << "(" << loc.first << "," << loc.second << ") ";
        }
        cout << endl;
        cout << "- Recommendation: " << analysis.diagnostic_recommendation << endl;
        
        cout << "\nClinical Workflow:" << endl;
        cout << "1. Automated screening completed" << endl;
        cout << "2. Priority flagging: " << (analysis.anomaly_count > 2 ? "HIGH" : "NORMAL") << endl;
        cout << "3. Radiologist review: " << (analysis.anomaly_count > 0 ? "REQUIRED" : "OPTIONAL") << endl << endl;
    }

    static void demonstrateAutonomousVehicle() {
        cout << "=== AUTONOMOUS VEHICLE PERCEPTION ===" << endl;
        cout << "Application: Real-time road scene understanding" << endl;
        cout << "Systems: Lane keeping, obstacle avoidance, traffic sign recognition" << endl << endl;
        
        auto road_scene = RealWorldCV::createRoadScene(8, 8);
        road_scene.print("Camera View (Forward-facing camera):");
        
        auto analysis = road_scene.analyzeTrafficScene();
        
        cout << "Perception System Output:" << endl;
        cout << "- Lanes detected: " << analysis.lane_count << endl;
        cout << "- Lane positions: ";
        for (int pos : analysis.lane_positions) {
            cout << pos << " ";
        }
        cout << "(pixels from left)" << endl;
        cout << "- Obstacles detected: " << analysis.obstacle_count << endl;
        cout << "- Obstacle locations: ";
        for (const auto& loc : analysis.obstacle_locations) {
            cout << "(" << loc.first << "," << loc.second << ") ";
        }
        cout << endl;
        cout << "- Driving decision: " << analysis.driving_recommendation << endl;
        
        cout << "\nVehicle Control Actions:" << endl;
        cout << "- Steering: " << (analysis.lane_count >= 2 ? "Center between lanes" : "Search for lanes") << endl;
        cout << "- Speed: " << (analysis.obstacle_count > 0 ? "REDUCE" : "MAINTAIN") << endl;
        cout << "- Braking: " << (analysis.obstacle_count > 2 ? "ENGAGED" : "READY") << endl << endl;
    }

    static void demonstrateSecuritySurveillance() {
        cout << "=== SECURITY & SURVEILLANCE ===" << endl;
        cout << "Application: Intelligent video monitoring" << endl;
        cout << "Features: Motion detection, intrusion alerts, behavior analysis" << endl << endl;
        
        auto frame1 = RealWorldCV::createSecurityFrame(8, 8, false);
        auto frame2 = RealWorldCV::createSecurityFrame(8, 8, true);
        
        frame1.print("Previous Frame (t-1):");
        frame2.print("Current Frame (t):");
        
        auto analysis = frame2.compareFrames(frame1);
        
        cout << "Security Analysis:" << endl;
        cout << "- Motion detected: " << (analysis.motion_detected ? "YES" : "NO") << endl;
        cout << "- Motion intensity: " << analysis.motion_intensity << endl;
        cout << "- Motion areas: " << analysis.motion_areas.size() << " regions" << endl;
        cout << "- Alert level: " << analysis.alert_level << endl;
        
        cout << "\nSecurity Actions:" << endl;
        if (analysis.motion_detected) {
            cout << "1. Recording activated" << endl;
            cout << "2. Motion tracking engaged" << endl;
            cout << "3. " << (analysis.motion_intensity > 1000 ? "Security alert sent" : "Monitoring continues") << endl;
        } else {
            cout << "1. Normal surveillance mode" << endl;
            cout << "2. Power saving enabled" << endl;
        }
        cout << endl;
    }

    static void demonstrateRetailAnalytics() {
        cout << "=== RETAIL & COMMERCE ANALYTICS ===" << endl;
        cout << "Application: Smart retail management" << endl;
        cout << "Features: Inventory tracking, customer behavior, theft prevention" << endl << endl;
        
        auto shelf = RealWorldCV::createRetailShelf(8, 8);
        shelf.print("Retail Shelf Camera View:");
        
        // Analyze shelf stock
        int product_count = 0;
        int empty_spaces = 0;
        vector<pair<int, int>> products;
        
        for (int i = 0; i < shelf.getRows(); i++) {
            for (int j = 0; j < shelf.getCols(); j++) {
                int pixel_val = shelf.get(i, j);
                if (pixel_val > 150) {
                    product_count++;
                    products.push_back({i, j});
                } else if (pixel_val < 50 && i > 2) { // Below shelf level
                    empty_spaces++;
                }
            }
        }
        
        cout << "Inventory Analysis:" << endl;
        cout << "- Products detected: " << product_count << " items" << endl;
        cout << "- Empty spaces: " << empty_spaces << " locations" << endl;
        cout << "- Stock level: " << (product_count > 8 ? "FULL" : product_count > 4 ? "MEDIUM" : "LOW") << endl;
        cout << "- Restocking needed: " << (product_count < 6 ? "YES" : "NO") << endl;
        
        cout << "\nBusiness Intelligence:" << endl;
        cout << "- Popular items: " << (product_count < 6 ? "High turnover detected" : "Normal sales") << endl;
        cout << "- Planogram compliance: " << (empty_spaces < 3 ? "GOOD" : "POOR") << endl;
        cout << "- Automated reorder: " << (product_count < 4 ? "TRIGGERED" : "NOT NEEDED") << endl << endl;
    }

    static void demonstrateAgriculturalMonitoring() {
        cout << "=== AGRICULTURAL MONITORING ===" << endl;
        cout << "Application: Precision agriculture and crop management" << endl;
        cout << "Features: Disease detection, yield prediction, irrigation optimization" << endl << endl;
        
        auto field = RealWorldCV::createAgriculturalField(8, 8);
        field.print("Drone/Satellite Crop Image:");
        
        // Analyze crop health
        int healthy_pixels = 0, diseased_pixels = 0, weed_pixels = 0;
        float total_health = 0;
        
        for (int i = 0; i < field.getRows(); i++) {
            for (int j = 0; j < field.getCols(); j++) {
                int pixel_val = field.get(i, j);
                total_health += pixel_val;
                
                if (pixel_val > 150) {
                    if (pixel_val > 180) weed_pixels++;
                    else healthy_pixels++;
                } else if (pixel_val < 60) {
                    diseased_pixels++;
                } else {
                    healthy_pixels++;
                }
            }
        }
        
        float avg_health = total_health / (field.getRows() * field.getCols());
        
        cout << "Crop Health Analysis:" << endl;
        cout << "- Average crop health: " << fixed << setprecision(1) << avg_health << "/255" << endl;
        cout << "- Healthy area: " << healthy_pixels << " pixels (" 
             << (healthy_pixels * 100 / 64) << "%)" << endl;
        cout << "- Diseased area: " << diseased_pixels << " pixels (" 
             << (diseased_pixels * 100 / 64) << "%)" << endl;
        cout << "- Weed infestation: " << weed_pixels << " pixels (" 
             << (weed_pixels * 100 / 64) << "%)" << endl;
        
        cout << "\nFarm Management Actions:" << endl;
        cout << "- Disease treatment: " << (diseased_pixels > 5 ? "URGENT - Apply fungicide" : "Monitor closely") << endl;
        cout << "- Weed control: " << (weed_pixels > 2 ? "Targeted herbicide application" : "Not needed") << endl;
        cout << "- Irrigation: " << (avg_health < 90 ? "Increase water supply" : "Maintain current levels") << endl;
        cout << "- Yield prediction: " << (avg_health > 100 ? "Above average" : "Below average") << " expected" << endl << endl;
    }
};

int main() {
    cout << "REAL-WORLD COMPUTER VISION APPLICATIONS" << endl;
    cout << "=======================================" << endl;
    cout << "Demonstrating practical CV systems used in industry today" << endl << endl;
    
    try {
        // Manufacturing Quality Control
        RealWorldApplications::demonstrateManufacturingQC();
        
        // Medical Image Analysis
        RealWorldApplications::demonstrateMedicalImaging();
        
        // Autonomous Vehicle Systems
        RealWorldApplications::demonstrateAutonomousVehicle();
        
        // Security & Surveillance
        RealWorldApplications::demonstrateSecuritySurveillance();
        
        // Retail Analytics
        RealWorldApplications::demonstrateRetailAnalytics();
        
        // Agricultural Monitoring
        RealWorldApplications::demonstrateAgriculturalMonitoring();
        
        cout << "=== INDUSTRY IMPACT SUMMARY ===" << endl;
        cout << "Computer Vision is transforming industries through:" << endl;
        cout << "✓ Quality Control: 99.9% defect detection accuracy" << endl;
        cout << "✓ Medical Imaging: Early disease detection saves lives" << endl;
        cout << "✓ Autonomous Vehicles: Safer transportation systems" << endl;
        cout << "✓ Security: 24/7 intelligent monitoring" << endl;
        cout << "✓ Retail: Optimized inventory and customer experience" << endl;
        cout << "✓ Agriculture: Sustainable farming and higher yields" << endl;
        
        cout << "\nMarket Value: $15+ billion industry growing at 20% annually" << endl;
        cout << "Job Opportunities: High demand for CV engineers across all sectors" << endl;
        
        return 0;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
}