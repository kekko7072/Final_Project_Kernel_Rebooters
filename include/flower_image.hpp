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
    FlowerImage();

    const cv::Mat& getImageColor() const;
    cv::Mat& getImageColor();
    const cv::Mat& getImageGrayscale() const;
    cv::Mat& getImageGrayscale();

    const std::string& name() const;
    const FlowerType& flowerType() const;
    bool isHealthy() const;
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
