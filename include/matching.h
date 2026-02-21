// Author: Francesco Vezzani

#ifndef MATCHING_H
#define MATCHING_H

#include <flower_image_container.hpp>
#include <string>

void hog(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy_images,
    const FlowerImageContainer& train_diseased_images,
    const std::string& output_dir
);

void bow(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy_images,
    const FlowerImageContainer& train_diseased_images,
    const std::string& output_dir
);

#endif // MATCHING_H
