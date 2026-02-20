// Author: Luca Pellegrini
#include "flower_image.hpp"

FlowerImage::FlowerImage(
    std::string name, FlowerType fl_type, bool healthy, int img_type,
    cv::Mat_<cv::Vec3b>& img_color, cv::Mat_<uchar>& img_gray) :
    m_name{name}, m_fl_type{fl_type}, m_healthy{healthy}, m_image_type{img_type}
{
    m_image_color = img_color;
    m_image_grayscale = img_gray;
}

const cv::Mat_<cv::Vec3b>& FlowerImage::getImageColor() const
{
    return m_image_color;
}

cv::Mat_<cv::Vec3b>& FlowerImage::getImageColor()
{
    return m_image_color;
}

const cv::Mat_<uchar>& FlowerImage::getImageGrayscale() const
{
    return m_image_grayscale;
}

cv::Mat_<uchar>& FlowerImage::getImageGrayscale()
{
    return m_image_grayscale;
}

const std::string& FlowerImage::name() const
{
    return m_name;
}

const FlowerType& FlowerImage::flowerType() const
{
    return m_fl_type;
}

bool FlowerImage::isHealthy() const
{
    return m_healthy;
}

int FlowerImage::imageType() const
{
    return m_image_type;
}

bool FlowerImage::isTrain() const
{
    return (m_image_type == 0);
}

bool FlowerImage::isTest() const
{
    return (m_image_type == 1);
}
