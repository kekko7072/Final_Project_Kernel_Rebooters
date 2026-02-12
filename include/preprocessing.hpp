// Author: Luca Pellegrini
#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

#include <vector>

#include "flower_type.hpp"
#include "flower_image.hpp"

/**
 * @brief load_images Load train and test images (from user-supplied or default path)
 * @param train_healthy_imgs  Output param, vector of train healthy flower images
 * @param train_diseased_imgs Output param, vector of train diseased flower images
 * @param test_imgs Output param, vector of test images
 * @return true if successful; false otherwise
 */
bool load_images(std::vector<FlowerImage> test_imgs,
                 std::vector<FlowerImage> train_healthy_imgs,
                 std::vector<FlowerImage> train_diseased_imgs);

#endif // PREPROCESSING_HPP
