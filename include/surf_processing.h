// Author: Marco Carraro

#ifndef SURF_PROCESSING_H
#define SURF_PROCESSING_H

#ifdef ENABLE_SURF

#include <opencv2/opencv.hpp>
#include <vector>
#include <map>
#include <string>

#include "flower_type.hpp"
#include "flower_image.hpp"
#include "flower_image_container.hpp"
#include "surf.h"
#include "metrics.h"

// Extract SURF features from a container and store them in a temporary map
void extractSURFFeaturesFromContainer(
    const FlowerImageContainer& images,
    SURFExtractor& surf_extractor,
    std::map<FlowerType, std::vector<cv::Mat>>& temp_descriptors
);

// Combine descriptors from multiple images of the same class into a single matrix
void combineSURFDescriptors(
    const std::map<FlowerType, std::vector<cv::Mat>>& temp_descriptors,
    std::map<FlowerType, cv::Mat>& train_descriptors,
    const std::vector<std::string>& class_names
);

// Train SURF on healthy and optionally diseased images
void trainSURF(
    const FlowerImageContainer& train_healthy,
    const FlowerImageContainer& train_diseased,
    SURFExtractor& surf_extractor,
    std::map<FlowerType, cv::Mat>& train_descriptors,
    const std::vector<std::string>& class_names,
    bool use_diseased = true
);

// Test SURF on test images and update metrics
void testSURF(
    const FlowerImageContainer& test_images,
    const std::map<FlowerType, cv::Mat>& train_descriptors,
    SURFExtractor& surf_extractor,
    Metrics& metrics,
    const std::vector<std::string>& class_names,
    double threshold,
    bool verbose = true
);

#endif // ENABLE_SURF
#endif // SURF_PROCESSING_H