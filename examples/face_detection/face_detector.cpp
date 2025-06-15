/**
 * @file face_detector.cpp
 * @brief Face detection using Haar cascades
 * 
 * This example demonstrates:
 * - Loading Haar cascade classifiers
 * - Face detection in images
 * - Drawing detection results
 * - Performance measurement
 */

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <chrono>

using namespace cv;
using namespace std;
using namespace std::chrono;

class FaceDetector {
private:
    CascadeClassifier face_cascade;
    CascadeClassifier eye_cascade;
    bool cascades_loaded;

public:
    /**
     * @brief Constructor - loads cascade classifiers
     * @param face_cascade_path Path to face cascade XML file
     * @param eye_cascade_path Path to eye cascade XML file (optional)
     */
    FaceDetector(const string& face_cascade_path, const string& eye_cascade_path = "") {
        cascades_loaded = false;
        
        // Load face cascade
        if (!face_cascade.load(face_cascade_path)) {
            cerr << "Error: Could not load face cascade from " << face_cascade_path << endl;
            return;
        }
        
        // Load eye cascade (optional)
        if (!eye_cascade_path.empty()) {
            if (!eye_cascade.load(eye_cascade_path)) {
                cout << "Warning: Could not load eye cascade from " << eye_cascade_path << endl;
            }
        }
        
        cascades_loaded = true;
        cout << "Cascade classifiers loaded successfully!" << endl;
    }

    /**
     * @brief Check if cascades are properly loaded
     * @return True if cascades are loaded
     */
    bool isReady() const {
        return cascades_loaded;
    }

    /**
     * @brief Detect faces in an image
     * @param image Input image
     * @param scale_factor Scale factor for detection
     * @param min_neighbors Minimum neighbors for detection
     * @param min_size Minimum face size
     * @return Vector of detected face rectangles
     */
    vector<Rect> detectFaces(const Mat& image, double scale_factor = 1.1, 
                           int min_neighbors = 3, Size min_size = Size(30, 30)) {
        if (!cascades_loaded) {
            return vector<Rect>();
        }

        vector<Rect> faces;
        Mat gray;
        
        // Convert to grayscale for detection
        if (image.channels() == 3) {
            cvtColor(image, gray, COLOR_BGR2GRAY);
        } else {
            gray = image.clone();
        }
        
        // Enhance contrast for better detection
        equalizeHist(gray, gray);
        
        // Detect faces
        auto start = high_resolution_clock::now();
        
        face_cascade.detectMultiScale(gray, faces, scale_factor, min_neighbors, 
                                    0 | CASCADE_SCALE_IMAGE, min_size);
        
        auto end = high_resolution_clock::now();
        auto duration = duration_cast<milliseconds>(end - start);
        
        cout << "Face detection took: " << duration.count() << " ms" << endl;
        cout << "Detected " << faces.size() << " faces" << endl;
        
        return faces;
    }

    /**
     * @brief Detect eyes within face regions
     * @param image Input image
     * @param face_rects Vector of face rectangles
     * @return Vector of eye rectangles
     */
    vector<Rect> detectEyes(const Mat& image, const vector<Rect>& face_rects) {
        vector<Rect> all_eyes;
        
        if (eye_cascade.empty()) {
            return all_eyes;
        }
        
        Mat gray;
        if (image.channels() == 3) {
            cvtColor(image, gray, COLOR_BGR2GRAY);
        } else {
            gray = image.clone();
        }
        
        // Detect eyes within each face region
        for (const auto& face : face_rects) {
            Mat face_roi = gray(face);
            vector<Rect> eyes;
            
            eye_cascade.detectMultiScale(face_roi, eyes, 1.1, 2, 
                                       0 | CASCADE_SCALE_IMAGE, Size(5, 5));
            
            // Adjust eye coordinates to full image
            for (auto& eye : eyes) {
                eye.x += face.x;
                eye.y += face.y;
                all_eyes.push_back(eye);
            }
        }
        
        cout << "Detected " << all_eyes.size() << " eyes" << endl;
        return all_eyes;
    }

    /**
     * @brief Draw detection results on image
     * @param image Input/output image
     * @param faces Vector of face rectangles
     * @param eyes Vector of eye rectangles
     * @return Image with drawn detections
     */
    static Mat drawDetections(Mat image, const vector<Rect>& faces, 
                            const vector<Rect>& eyes = vector<Rect>()) {
        Mat result = image.clone();
        
        // Draw face rectangles
        for (const auto& face : faces) {
            rectangle(result, face, Scalar(255, 0, 0), 2);
            
            // Add face label
            Point label_pos(face.x, face.y - 10);
            putText(result, "Face", label_pos, FONT_HERSHEY_SIMPLEX, 
                   0.7, Scalar(255, 0, 0), 2);
        }
        
        // Draw eye rectangles
        for (const auto& eye : eyes) {
            rectangle(result, eye, Scalar(0, 255, 0), 2);
            
            // Add eye label
            Point label_pos(eye.x, eye.y - 5);
            putText(result, "Eye", label_pos, FONT_HERSHEY_SIMPLEX, 
                   0.4, Scalar(0, 255, 0), 1);
        }
        
        // Add detection summary
        string summary = "Faces: " + to_string(faces.size()) + 
                        ", Eyes: " + to_string(eyes.size());
        putText(result, summary, Point(10, result.rows - 20), 
               FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
        
        return result;
    }

    /**
     * @brief Create a test image with simple face-like shapes
     * @return Generated test image
     */
    static Mat createTestFaceImage() {
        Mat image = Mat::zeros(400, 600, CV_8UC3);
        image.setTo(Scalar(50, 50, 50)); // Dark gray background
        
        // Draw simple face-like shapes
        // Face 1
        ellipse(image, Point(150, 150), Size(80, 100), 0, 0, 360, 
                Scalar(200, 180, 160), -1);
        circle(image, Point(130, 130), 8, Scalar(0, 0, 0), -1); // Left eye
        circle(image, Point(170, 130), 8, Scalar(0, 0, 0), -1); // Right eye
        ellipse(image, Point(150, 160), Size(15, 8), 0, 0, 360, 
                Scalar(100, 50, 50), -1); // Mouth
        
        // Face 2
        ellipse(image, Point(450, 200), Size(70, 90), 0, 0, 360, 
                Scalar(210, 190, 170), -1);
        circle(image, Point(435, 180), 7, Scalar(0, 0, 0), -1); // Left eye
        circle(image, Point(465, 180), 7, Scalar(0, 0, 0), -1); // Right eye
        ellipse(image, Point(450, 210), Size(12, 6), 0, 0, 360, 
                Scalar(100, 50, 50), -1); // Mouth
        
        // Add some noise and texture
        Mat noise = Mat::zeros(image.size(), CV_8UC3);
        randu(noise, Scalar(0, 0, 0), Scalar(30, 30, 30));
        add(image, noise, image);
        
        return image;
    }
};

int main(int argc, char** argv) {
    try {
        cout << "OpenCV Face Detection Demo" << endl;
        cout << "==========================" << endl;
        
        // Check for cascade files
        string face_cascade_path = "data/haarcascades/haarcascade_frontalface_default.xml";
        string eye_cascade_path = "data/haarcascades/haarcascade_eye.xml";
        
        // Initialize face detector
        FaceDetector detector(face_cascade_path, eye_cascade_path);
        
        if (!detector.isReady()) {
            cerr << "Failed to initialize face detector!" << endl;
            cerr << "Please ensure cascade files are available in data/haarcascades/" << endl;
            return -1;
        }
        
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
            cout << "Creating test image with face-like shapes..." << endl;
            image = FaceDetector::createTestFaceImage();
        }
        
        cout << "Image size: " << image.cols << "x" << image.rows << endl;
        
        // Detect faces
        cout << "Detecting faces..." << endl;
        vector<Rect> faces = detector.detectFaces(image);
        
        // Detect eyes
        cout << "Detecting eyes..." << endl;
        vector<Rect> eyes = detector.detectEyes(image, faces);
        
        // Draw results
        Mat result = FaceDetector::drawDetections(image, faces, eyes);
        
        // Save results
        imwrite("output_face_detection.jpg", result);
        imwrite("output_original_face.jpg", image);
        
        cout << endl << "Detection Results:" << endl;
        cout << "- Faces detected: " << faces.size() << endl;
        cout << "- Eyes detected: " << eyes.size() << endl;
        cout << "- Results saved to output_face_detection.jpg" << endl;
        
        // Print face coordinates
        for (size_t i = 0; i < faces.size(); i++) {
            cout << "Face " << i+1 << ": " 
                 << "x=" << faces[i].x << ", y=" << faces[i].y 
                 << ", w=" << faces[i].width << ", h=" << faces[i].height << endl;
        }
        
        return 0;
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return -1;
    }
}