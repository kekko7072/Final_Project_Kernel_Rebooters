// Author: Marco Carraro

#include "sift_processing.h"
#include "print_stats.h"
#include <iostream>
#include <chrono>
#include <filesystem>

namespace fs = std::filesystem;
using std::cout;
using std::endl;

void extractSIFTFeaturesFromContainer(
    const FlowerImageContainer& images,
    SIFTExtractor& sift_extractor,
    std::map<FlowerType, std::vector<cv::Mat>>& temp_descriptors)
{
    for (size_t i = 0; i < images.size(); i++) {
        const FlowerImage& img = images.at(i);
        
        std::vector<cv::KeyPoint> keypoints;
        cv::Mat descriptors;
        
        sift_extractor.extract(img.getImageGrayscale(), keypoints, descriptors);
        
        if (!descriptors.empty()) {
            temp_descriptors[img.flowerType()].push_back(descriptors);
        }
    }
}

void combineSIFTDescriptors(
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

void trainSIFT(
    const FlowerImageContainer& train_healthy,
    const FlowerImageContainer& train_diseased,
    SIFTExtractor& sift_extractor,
    std::map<FlowerType, cv::Mat>& train_descriptors,
    const std::vector<std::string>& class_names,
    bool use_diseased)
{
    cout << "\nSIFT Training:" << endl;
    cout << "Extracting SIFT features from training images..." << endl;
    
    std::map<FlowerType, std::vector<cv::Mat>> temp_descriptors;
    
    // Extract from healthy
    cout << "Processing healthy images..." << endl;
    extractSIFTFeaturesFromContainer(train_healthy, sift_extractor, temp_descriptors);
    
    // Extract from diseased (optional)
    if (use_diseased) {
        cout << "Processing diseased images..." << endl;
        extractSIFTFeaturesFromContainer(train_diseased, sift_extractor, temp_descriptors);
    }
    
    // Combine descriptors
    combineSIFTDescriptors(temp_descriptors, train_descriptors, class_names);
}

void testSIFT(
    const FlowerImageContainer& test_images,
    const std::map<FlowerType, cv::Mat>& train_descriptors,
    SIFTExtractor& sift_extractor,
    Metrics& metrics,
    const std::vector<std::string>& class_names,
    double threshold,
    ClassificationRecap* records,
    bool verbose)
{
    cout << "\nSIFT Testing: " << endl;
    cout << "Threshold: " << threshold << endl;
    cout << "Testing on " << test_images.size() << " images..." << endl;
        
    for (size_t i = 0; i < test_images.size(); i++) {
        const FlowerImage& test_img = test_images.at(i);
        
        std::vector<cv::KeyPoint> test_keypoints;
        cv::Mat test_descriptors;
        
        // Start timing
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Extract features
        sift_extractor.extract(test_img.getImageGrayscale(), test_keypoints, test_descriptors);
        
        if (test_descriptors.empty()) {
            if (verbose) {
                cout << "WARNING: No keypoints in " << test_img.name() << endl;
            }
            continue;
        }
        
        // Find best match
        FlowerType predicted_type = FlowerType::NoFlower;
        int max_matches = 0;
        
        for (const auto& [flower_type, train_desc] : train_descriptors) {
            std::vector<cv::DMatch> good_matches;
            sift_extractor.matchAndFilter(test_descriptors, train_desc, good_matches, threshold);
            
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

        ClassificationRecord record = {
            test_img.name(),                
            class_names[true_class],          
            class_names[predicted_class]       
        };
        records->push_back(record);
        
        if (verbose) {
            cout << "Image: " << test_img.name() 
                 << " | True: " << class_names[true_class]
                 << " | Predicted: " << class_names[predicted_class]
                // << " | Matches: " << max_matches     // DEBUG 
                 << " | Time: " << total_time << " ms" << endl;
        }
    }
}

void sift(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy,
    const FlowerImageContainer& train_diseased,
    const std::string& output_dir
) {
    SIFTExtractor sift;
    Metrics sift_metrics = createMetrics(6);
    std::map<FlowerType, cv::Mat> sift_train_descriptors;
    ClassificationRecap sift_records;
    
    // Train SIFT
    trainSIFT(train_healthy, train_diseased, sift, sift_train_descriptors, class_names, true);
    
    // Test SIFT
    double sift_threshold = 1.7;  // Can be tuned (higher = more matches, lower = stricter)
    testSIFT(test_images, sift_train_descriptors, sift, sift_metrics, class_names, sift_threshold, &sift_records, true);
    
    // Display results
    printClassificationReport(sift_metrics, class_names, "SIFT");

    // Save recap to file
    fs::path output_path = fs::path(output_dir) / "sift_recap.txt";
    saveClassificationRecap(sift_records, sift_metrics, class_names, "SIFT", output_path.string());
}