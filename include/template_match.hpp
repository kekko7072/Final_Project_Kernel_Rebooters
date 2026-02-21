// Author: Luca Pellegrini
#ifndef TEMPLATE_MATCH_HPP
#define TEMPLATE_MATCH_HPP

#include <filesystem>
#include <flower_image_container.hpp>
#include <flower_template.hpp>

/**
 * @brief Classifies test images with the Template Matching method
 *
 * To overcome the fact that test images could have different sizes and scales,
 * it builds a piramid of upscaled and downscaled images, and then applies the
 * template matching algorithm on the image with a suitable size (with respect
 * to the size of the template images).
 *
 * Only TM_SQDIFF and TM_CCORR_NORMED matching methods from the OpenCV library
 * support the use of a mask.
 *
 * This function is meant to be run in a separate thread, and it will update the
 * `success` shared variable upon successful completion (or failure).
 *
 * @param test_images Test images as loaded by `loadImages()`
 * @param output_dir where to store the Classification Recap file
 * @param success shared variable to comunicate exit status to parent thread
 */
void template_match(
    const FlowerImageContainer& test_images,
    const std::vector<FlowerTemplate>& daisy_templates,
    const std::vector<FlowerTemplate>& dandelion_templates,
    const std::vector<FlowerTemplate>& rose_templates,
    const std::vector<FlowerTemplate>& sunflower_templates,
    const std::vector<FlowerTemplate>& tulip_templates,
    const std::filesystem::path output_dir,
    bool& success
);

/**
 * @brief Compares a set of templates (of the same flower class) with the given image
 * @param image a suitable image (size must be greater than that of the templates)
 * @param templates as loaded by `loadTemplates()`
 * @return the highest similarity score achieved
 */
double processImage(
    const cv::Mat_<cv::Vec3b> image,
    const std::vector<FlowerTemplate>& templates
);

#endif // TEMPLATE_MATCH_HPP
