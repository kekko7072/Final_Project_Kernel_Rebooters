// Author: Marco Carraro

#include "orb.h"
#include <chrono>
#include <iostream>

ORBExtractor::ORBExtractor(
    int nfeatures,
    float scaleFactor,
    int nlevels,
    int edgeThreshold,
    int firstLevel,
    int WTA_K,
    int patchSize
) : extractionTime_(0.0), keypointCount_(0) {
    orb_ = cv::ORB::create(
        nfeatures,
        scaleFactor,
        nlevels,
        edgeThreshold,
        firstLevel,
        WTA_K,
        cv::ORB::HARRIS_SCORE,
        patchSize
    );
}

void ORBExtractor::extract(const cv::Mat &image, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors){
    // Check if the input image is empty
    if (image.empty()) {
        std::cerr << "[ORB ERROR] Input image is empty!" << std::endl;
        extractionTime_ = 0.0;
        keypointCount_ = 0;
        return;
    }
    
    // Start timing
    auto start = std::chrono::high_resolution_clock::now();
    
    // Extract ORB features
    orb_->detectAndCompute(image, cv::noArray(), keypoints, descriptors);
    
    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    extractionTime_ = std::chrono::duration<double>(end - start).count();
    keypointCount_ = static_cast<int>(keypoints.size());
}

// Get the time taken for the last extraction
double ORBExtractor::getExtractionTime() const {
    return extractionTime_;
}

// Get the number of keypoints detected in the last extraction
int ORBExtractor::getKeypointCount() const {
    return keypointCount_;
}