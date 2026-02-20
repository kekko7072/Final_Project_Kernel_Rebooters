// Author: Marco Carraro

#ifndef ORB_PROCESSING_H
#define ORB_PROCESSING_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <map>

#include "flower_type.hpp"
#include "flower_image.hpp"
#include "flower_image_container.hpp"
#include "orb.h"
#include "metrics.h"

// Extract ORB features from a container and store them in a temporary map
void extractORBFeaturesFromContainer(
    const FlowerImageContainer& images,
    ORBExtractor& orb_extractor,
    std::map<FlowerType, std::vector<cv::Mat>>& temp_descriptors
);

// Combine descriptors from multiple images of the same class into a single matrix
void combineORBDescriptors(
    const std::map<FlowerType, std::vector<cv::Mat>>& temp_descriptors,
    std::map<FlowerType, cv::Mat>& train_descriptors,
    const std::vector<std::string>& class_names
);

// Train ORB on healthy and optionally diseased images
void trainORB(
    const FlowerImageContainer& train_healthy,
    const FlowerImageContainer& train_diseased,
    ORBExtractor& orb_extractor,
    std::map<FlowerType, cv::Mat>& train_descriptors,
    const std::vector<std::string>& class_names,
    bool use_diseased = true
);

// Test ORB on test images and update metrics
void testORB(
    const FlowerImageContainer& test_images,
    const std::map<FlowerType, cv::Mat>& train_descriptors,
    ORBExtractor& orb_extractor,
    Metrics& metrics,
    const std::vector<std::string>& class_names,
    double threshold,
    bool verbose = true
);

// Wrapper function to run the entire ORB pipeline (training + testing)
void orb(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy,
    const FlowerImageContainer& train_diseased
);

#endif // ORB_PROCESSING_H