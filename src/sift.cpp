// Author: Marco Carraro

#include "sift.h"
#include <chrono>
#include <iostream>
#include <algorithm>

SIFTExtractor::SIFTExtractor(
    int nfeatures,
    int nOctaveLayers,
    double contrastThreshold,
    double edgeThreshold,
    double sigma
) : extractionTime_(0.0), keypointCount_(0), matchingTime_(0.0) {
    sift_ = cv::SIFT::create(
        nfeatures,
        nOctaveLayers,
        contrastThreshold,
        edgeThreshold,
        sigma
    );

    // Initialize the BFMacher with L2 distance
    matcher_ = cv::FlannBasedMatcher::create();
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

int SIFTExtractor::matchDescriptors(const cv::Mat &descriptors1, const cv::Mat &descriptors2, std::vector<cv::DMatch> &matches){
    // Check if descriptors are empty
    if (descriptors1.empty() || descriptors2.empty()) {
        std::cerr << "[SIFT ERROR] Empty descriptors for matching!" << std::endl;
        matchingTime_ = 0.0;
        return 0;
    }
    
    // Start timing
    auto start = std::chrono::high_resolution_clock::now();
    
    // Match descriptors using the BFMatcher
    matcher_->match(descriptors1, descriptors2, matches);
    
    // End timing
    auto end = std::chrono::high_resolution_clock::now();
    matchingTime_ = std::chrono::duration<double, std::milli>(end - start).count();

    return static_cast<int>(matches.size());
}

std::vector<cv::DMatch> SIFTExtractor::filterMatches(const std::vector<cv::DMatch> &matches, double threshold){
    // Check if there are matches to filter
    if (matches.empty()) {
        std::cout << "[SIFT] No matches to filter" << std::endl;
        return std::vector<cv::DMatch>();
    }
    
    // Find the minimum distance among matches
    double min_distance = matches[0].distance;
    for (const auto& match : matches) {
        if (match.distance < min_distance) {
            min_distance = match.distance;
        }
    }
    
    // Filter matches based on the distance threshold
    std::vector<cv::DMatch> goodMatches;
    double distance_threshold = threshold * min_distance;
    
    for (const auto& match : matches) {
        if (match.distance < distance_threshold) {
            goodMatches.push_back(match);
        }
    }
    
    return goodMatches;
}

int SIFTExtractor::matchAndFilter(const cv::Mat &descriptors1, const cv::Mat &descriptors2, std::vector<cv::DMatch> &goodMatches, double threshold){
    // Match
    std::vector<cv::DMatch> allMatches;
    matchDescriptors(descriptors1, descriptors2, allMatches);
    
    // Filter
    goodMatches = filterMatches(allMatches, threshold);
    
    return static_cast<int>(goodMatches.size());
}

// Get the time taken for the last extraction
double SIFTExtractor::getExtractionTime() const {
    return extractionTime_;
}

// Get the number of keypoints detected in the last extraction
int SIFTExtractor::getKeypointCount() const {
    return keypointCount_;
}

// Get the time taken for the last matching operation
double SIFTExtractor::getMatchingTime() const {
    return matchingTime_;
}