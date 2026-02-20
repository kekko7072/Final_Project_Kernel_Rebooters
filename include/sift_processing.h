// Author: Marco Carraro

#ifndef SIFT_PROCESSING_H
#define SIFT_PROCESSING_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <map>

#include "flower_type.hpp"
#include "flower_image.hpp"
#include "flower_image_container.hpp"
#include "sift.h"
#include "metrics.h"

// Extract SIFT features from a container and store them in a temporary map
void extractSIFTFeaturesFromContainer(
    const FlowerImageContainer& images,
    SIFTExtractor& sift_extractor,
    std::map<FlowerType, std::vector<cv::Mat>>& temp_descriptors
);

// Combine descriptors from multiple images of the same class into a single matrix
void combineSIFTDescriptors(
    const std::map<FlowerType, std::vector<cv::Mat>>& temp_descriptors,
    std::map<FlowerType, cv::Mat>& train_descriptors,
    const std::vector<std::string>& class_names
);

// Train SIFT on healthy and optionally diseased images
void trainSIFT(
    const FlowerImageContainer& train_healthy,
    const FlowerImageContainer& train_diseased,
    SIFTExtractor& sift_extractor,
    std::map<FlowerType, cv::Mat>& train_descriptors,
    const std::vector<std::string>& class_names,
    bool use_diseased = true
);

// Test SIFT on test images and update metrics
void testSIFT(
    const FlowerImageContainer& test_images,
    const std::map<FlowerType, cv::Mat>& train_descriptors,
    SIFTExtractor& sift_extractor,
    Metrics& metrics,
    const std::vector<std::string>& class_names,
    double threshold,
    bool verbose = true
);

// Wrapper function to run the entire SIFT pipeline (training + testing)
void sift(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy,
    const FlowerImageContainer& train_diseased
);

#endif // SIFT_PROCESSING_H