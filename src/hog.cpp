// Author: Francesco Vezzani

#include "hog.h"
#include <chrono>
#include <iostream>
#include <cmath>
#include <limits>

HOGExtractor::HOGExtractor(
    cv::Size winSize,
    cv::Size blockSize,
    cv::Size blockStride,
    cv::Size cellSize,
    int nbins
) : extractionTime_(0.0), descriptorCount_(0), matchingTime_(0.0) {
    hog_ = cv::HOGDescriptor(winSize, blockSize, blockStride, cellSize, nbins);
}

void HOGExtractor::extract(const cv::Mat &image, std::vector<float> &descriptors){
    if (image.empty()) {
        std::cerr << "[HOG ERROR] Input image is empty!" << std::endl;
        descriptors.clear();
        extractionTime_ = 0.0;
        descriptorCount_ = 0;
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    cv::Mat gray;
    if (image.channels() == 1) {
        gray = image;
    } else {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }

    cv::Mat resized;
    cv::resize(gray, resized, hog_.winSize);
    hog_.compute(resized, descriptors);

    auto end = std::chrono::high_resolution_clock::now();
    extractionTime_ = std::chrono::duration<double>(end - start).count();
    descriptorCount_ = static_cast<int>(descriptors.size());
}

double HOGExtractor::matchDescriptors(const std::vector<float> &descriptors1, const std::vector<float> &descriptors2){
    if (descriptors1.empty() || descriptors2.empty() || descriptors1.size() != descriptors2.size()) {
        std::cerr << "[HOG ERROR] Invalid descriptors for matching!" << std::endl;
        matchingTime_ = 0.0;
        return std::numeric_limits<double>::max();
    }

    auto start = std::chrono::high_resolution_clock::now();

    double sum = 0.0;
    for (size_t i = 0; i < descriptors1.size(); i++) {
        const double diff = static_cast<double>(descriptors1[i]) - static_cast<double>(descriptors2[i]);
        sum += diff * diff;
    }
    const double distance = std::sqrt(sum);

    auto end = std::chrono::high_resolution_clock::now();
    matchingTime_ = std::chrono::duration<double, std::milli>(end - start).count();

    return distance;
}

double HOGExtractor::getExtractionTime() const {
    return extractionTime_;
}

int HOGExtractor::getDescriptorCount() const {
    return descriptorCount_;
}

double HOGExtractor::getMatchingTime() const {
    return matchingTime_;
}
