// Author: Francesco Vezzani

#ifndef MATCHING_H
#define MATCHING_H

#include <flower_image_container.hpp>

void hog(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy_images,
    const FlowerImageContainer& train_diseased_images
);

void bow(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy_images,
    const FlowerImageContainer& train_diseased_images
);

#endif // MATCHING_H
