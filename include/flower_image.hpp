// Author: Luca Pellegrini
#ifndef FLOWER_IMAGE_HPP
#define FLOWER_IMAGE_HPP

#include <string>
#include <opencv2/core.hpp>

#include "flower_type.hpp"

class FlowerImage
{
public:
    FlowerImage(std::string name, FlowerType fl_type, bool healthy, int img_type,
                cv::Mat img_color, cv::Mat img_gray);

    const cv::Mat& getImageColor() const;
    cv::Mat& getImageColor();
    const cv::Mat& getImageGrayscale() const;
    cv::Mat& getImageGrayscale();

    // Public member variables
    const std::string name;
    const FlowerType flower_type;
    const bool healthy;
    const int image_type;  // 0 for train, 1 for test

private:
    cv::Mat m_image_color;
    cv::Mat m_image_grayscale;
};

#endif // FLOWER_IMAGE_HPP
