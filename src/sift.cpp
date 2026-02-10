// Author: Marco Carraro

#include "sift.h"
#include <chrono>
#include <iostream>

SIFTExtractor::SIFTExtractor(
    int nfeatures,
    int nOctaveLayers,
    double contrastThreshold,
    double edgeThreshold,
    double sigma
) : extractionTime_(0.0), keypointCount_(0) {
    
    sift_ = cv::SIFT::create(
        nfeatures,
        nOctaveLayers,
        contrastThreshold,
        edgeThreshold,
        sigma
    );
}

void SIFTExtractor::extract(const cv::Mat &image, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors){
    // Check if the input image is empty
    if (image.empty()) {
        std::cerr << "[SIFT ERROR] Input image is empty!" << std::endl;
        extractionTime_ = 0.0;
        keypointCount_ = 0;
        return;
    }

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Extract SIFT features
    sift_->detectAndCompute(image, cv::noArray(), keypoints, descriptors);

    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    extractionTime_ = std::chrono::duration<double>(end - start).count();
    keypointCount_ = static_cast<int>(keypoints.size());
}

// Get the time taken for the last extraction
double SIFTExtractor::getExtractionTime() const {
    return extractionTime_;
}

// Get the number of keypoints detected in the last extraction
int SIFTExtractor::getKeypointCount() const {
    return keypointCount_;
}