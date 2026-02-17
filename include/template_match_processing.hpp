// Author: Luca Pellegrini
#ifndef TEMPLATE_MATCH_PROCESSING_HPP
#define TEMPLATE_MATCH_PROCESSING_HPP

#include <flower_image_container.hpp>

/**
 * @brief Classifies test images with the Template Matching method
 *
 * To overcome the fact that test images could have different sizes and scales,
 * it builds a piramid of upscaled and downscaled images, and then applies the
 * template matching algorithm on the image with a suitable size (with respect
 * to the size of the template images).
 *
 * This function is meant to be run in a separate thread, and it will update the
 * `success` shared variable upon successful completion (or failure).
 *
 * @param test_images
 * @param template_images
 * @param t_mask_images
 * @param success shared variable to comunicate exit status to parent thread
 */
void template_match(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& template_images,
    const FlowerImageContainer& t_mask_images,
    bool& success
);

#endif // TEMPLATE_MATCH_PROCESSING_HPP
