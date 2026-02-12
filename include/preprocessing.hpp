// Author: Luca Pellegrini
#ifndef PREPROCESSING_HPP
#define PREPROCESSING_HPP

#include <filesystem>

#include <flower_type.hpp>
#include <flower_image.hpp>
#include <flower_image_container.hpp>

/**
 * @brief loadImages Load train and test images (from user-supplied or default path)
 * @param data_path           Path to train/test dataset
 * @param train_healthy_imgs  Output param, vector of train healthy flower images
 * @param train_diseased_imgs Output param, vector of train diseased flower images
 * @param test_imgs           Output param, vector of test images
 * @return true if successful; false otherwise
 */
bool loadImages(
    const std::filesystem::path& data_path,
    FlowerImageContainer& test_imgs,
    FlowerImageContainer& train_healthy_imgs,
    FlowerImageContainer& train_diseased_imgs
);

bool loadImagesFromDataset(
    const std::filesystem::path& dir_path,
    const bool healthy,
    const int image_type,
    FlowerImageContainer& imgs
);

bool isImage(const std::filesystem::path& file_path);

#endif // PREPROCESSING_HPP
