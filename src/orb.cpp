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
) : extractionTime_(0.0), keypointCount_(0), matchingTime_(0.0) {
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

    // Initialize the BFMatcher with Hamming distance for ORB descriptors
    matcher_ = cv::BFMatcher::create(cv::NORM_HAMMING, false);
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

int ORBExtractor::matchDescriptors(const cv::Mat &descriptors1, const cv::Mat &descriptors2, std::vector<cv::DMatch> &matches){
    // Check if descriptors are empty
    if (descriptors1.empty() || descriptors2.empty()) {
        std::cerr << "[ORB ERROR] Empty descriptors for matching!" << std::endl;
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

std::vector<cv::DMatch> ORBExtractor::filterMatches(const std::vector<cv::DMatch> &matches, double threshold){
    // Check if there are matches to filter
    if (matches.empty()) {
        std::cout << "[ORB] No matches to filter" << std::endl;
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

int ORBExtractor::matchAndFilter(const cv::Mat &descriptors1, const cv::Mat &descriptors2, std::vector<cv::DMatch> &goodMatches, double threshold){
    // Match
    std::vector<cv::DMatch> allMatches;
    matchDescriptors(descriptors1, descriptors2, allMatches);
    
    // Filter
    goodMatches = filterMatches(allMatches, threshold);
    
    return static_cast<int>(goodMatches.size());
}

// Get the time taken for the last extraction
double ORBExtractor::getExtractionTime() const {
    return extractionTime_;
}

// Get the number of keypoints detected in the last extraction
int ORBExtractor::getKeypointCount() const {
    return keypointCount_;
}

// Get the time taken for the last matching operation
double ORBExtractor::getMatchingTime() const {
    return matchingTime_;
}