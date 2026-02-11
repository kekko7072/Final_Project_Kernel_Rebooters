// Author: Marco Carraro

#include "surf.h"
#include <chrono>
#include <iostream>

SURFExtractor::SURFExtractor(
    double hessianThreshold,
    int nOctaves,
    int nOctaveLayers,
    bool extended,
    bool upright
) : extractionTime_(0.0), keypointCount_(0) {
    surf_ = cv::xfeatures2d::SURF::create(
        hessianThreshold,
        nOctaves,
        nOctaveLayers,
        extended,
        upright
    );
}

void SURFExtractor::extract(const cv::Mat &image, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors){
    // Check if the input image is empty
    if (image.empty()) {
        std::cerr << "[SURF ERROR] Input image is empty!" << std::endl;
        extractionTime_ = 0.0;
        keypointCount_ = 0;
        return;
    }
    
    // Start timing
    auto start = std::chrono::high_resolution_clock::now();
    
    // Extract SURF features
    surf_->detectAndCompute(image, cv::noArray(), keypoints, descriptors);
    
    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    extractionTime_ = std::chrono::duration<double>(end - start).count();
    keypointCount_ = static_cast<int>(keypoints.size());
}

// Get the time taken for the last extraction
double SURFExtractor::getExtractionTime() const {
    return extractionTime_;
}

// Get the number of keypoints detected in the last extraction
int SURFExtractor::getKeypointCount() const {
    return keypointCount_;
}