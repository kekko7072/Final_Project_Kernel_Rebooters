// Author: Marco Carraro

#include "orb_processing.h"
#include <iostream>
#include <chrono>

using std::cout;
using std::endl;

void extractORBFeaturesFromContainer(
    const FlowerImageContainer& images,
    ORBExtractor& orb_extractor,
    std::map<FlowerType, std::vector<cv::Mat>>& temp_descriptors)
{
    for (size_t i = 0; i < images.size(); i++) {
        const FlowerImage& img = images.at(i);
        
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        
        orb_extractor.extract(img.getImageGrayscale(), keypoints, descriptors);
        
        if (!descriptors.empty()) {
            temp_descriptors[img.flowerType()].push_back(descriptors);
        }
    }
}

void combineORBDescriptors(
    const std::map<FlowerType, std::vector<cv::Mat>>& temp_descriptors,
    std::map<FlowerType, cv::Mat>& train_descriptors,
    const std::vector<std::string>& class_names)
{
    cout << "\nCombining training descriptors per class..." << endl;
    
    for (const auto& [flower_type, desc_vec] : temp_descriptors) {
        cv::Mat combined;
        for (const auto& desc : desc_vec) {
            if (combined.empty()) {
                combined = desc.clone();
            } else {
                cv::vconcat(combined, desc, combined);
            }
        }
        
        train_descriptors[flower_type] = combined;
        
        int idx = static_cast<int>(flower_type);
        cout << class_names[idx] 
             << ": " << combined.rows << " descriptors" << endl;
    }
}

void trainORB(
    const FlowerImageContainer& train_healthy,
    const FlowerImageContainer& train_diseased,
    ORBExtractor& orb_extractor,
    std::map<FlowerType, cv::Mat>& train_descriptors,
    const std::vector<std::string>& class_names,
    bool use_diseased)
{
    cout << "\nORB Training:" << endl;
    cout << "Extracting ORB features from training images..." << endl;
    
    std::map<FlowerType, std::vector<cv::Mat>> temp_descriptors;
    
    cout << "Processing healthy images..." << endl;
    extractORBFeaturesFromContainer(train_healthy, orb_extractor, temp_descriptors);
    
    if (use_diseased) {
        cout << "Processing diseased images..." << endl;
        extractORBFeaturesFromContainer(train_diseased, orb_extractor, temp_descriptors);
    }
    
    combineORBDescriptors(temp_descriptors, train_descriptors, class_names);
}

void testORB(
    const FlowerImageContainer& test_images,
    const std::map<FlowerType, cv::Mat>& train_descriptors,
    ORBExtractor& orb_extractor,
    Metrics& metrics,
    const std::vector<std::string>& class_names,
    double threshold,
    bool verbose)
{
    cout << "\nORB Testing:" << endl;
    cout << "Threshold: " << threshold << endl;
    cout << "Testing on " << test_images.size() << " images..." << endl;
    
    for (size_t i = 0; i < test_images.size(); i++) {
        const FlowerImage& test_img = test_images.at(i);
        
        std::vector<cv::KeyPoint> test_keypoints;
        cv::Mat test_descriptors;
        
        // Start timing
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Extract ORB features from the test image
        orb_extractor.extract(test_img.getImageGrayscale(), test_keypoints, test_descriptors);
        
        if (test_descriptors.empty()) {
            if (verbose) {
                cout << "WARNING: No keypoints in " << test_img.name() << endl;
            }
            continue;
        }
        
        // Find best matching class
        FlowerType predicted_type = FlowerType::NoFlower;
        int max_matches = 0;
        
        for (const auto& [flower_type, train_desc] : train_descriptors) {
            std::vector<cv::DMatch> good_matches;
            orb_extractor.matchAndFilter(test_descriptors, train_desc, good_matches, threshold);
            
            if (static_cast<int>(good_matches.size()) > max_matches) {
                max_matches = static_cast<int>(good_matches.size());
                predicted_type = flower_type;
            }
        }
        
        // End timing
        auto end_time = std::chrono::high_resolution_clock::now();
        double total_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        
        // Update metrics
        int true_class = static_cast<int>(test_img.flowerType());
        int predicted_class = static_cast<int>(predicted_type);
        
        addPrediction(metrics, true_class, predicted_class);
        addProcessingTime(metrics, total_time);
        
        if (verbose) {
            cout << "Image: " << test_img.name() 
                 << " | True: " << class_names[true_class]
                 << " | Predicted: " << class_names[predicted_class]
                 << " | Matches: " << max_matches 
                 << " | Time: " << total_time << " ms" << endl;
        }
    }
}