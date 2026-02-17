// Author: Luca Pellegrini
#ifndef FLOWER_IMAGE_HPP
#define FLOWER_IMAGE_HPP

#include <string>
#include <opencv2/core.hpp>

#include "flower_type.hpp"

/**
 * @brief Data structure that represents a train or test image for the flower_classifier
 *
 * FlowerImage object store both a BGR, 3-channel version and a Grayscale, 1-channel version
 * of the corresponding source image. These can be accessed with dedicated getter functions.
 *
 * The only member variables that can be modified are the cv::Mat BGR and grayscale images,
 * whereas the metadata fields (name, flower type, etc.) are read-only (via getter functions).
 */
class FlowerImage
{
public:
    FlowerImage(std::string name, FlowerType fl_type, bool healthy, int img_type,
                cv::Mat img_color, cv::Mat img_gray);
    FlowerImage();

    /**
     * @brief Returns the BGR color version (3 channels) of the image
     */
    const cv::Mat& getImageColor() const;
    cv::Mat& getImageColor();

    /**
     * @brief Returns the Grayscale version (1 channel) of the image
     */
    const cv::Mat& getImageGrayscale() const;
    cv::Mat& getImageGrayscale();

    const std::string& name() const;
    const FlowerType& flowerType() const;
    bool isHealthy() const;
    /**
     * @brief Returns 0 if train image; 1 if test image
     */
    int imageType() const;
    bool isTrain() const;
    bool isTest() const;

private:
    std::string m_name;
    FlowerType  m_fl_type;
    bool m_healthy;
    int  m_image_type;  // 0 for train, 1 for test

    cv::Mat m_image_color;
    cv::Mat m_image_grayscale;
};

#endif // FLOWER_IMAGE_HPP
