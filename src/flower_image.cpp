// Author: Luca Pellegrini
#include "flower_image.hpp"

FlowerImage::FlowerImage(cv::Mat img_color, cv::Mat img_gray, std::string name,
                         FlowerType f_type, bool healthy, int i_type) :
    m_image_color{img_color}, m_image_grayscale{img_gray}, name{name}, flower_type{f_type}, healthy{healthy}, image_type{i_type}
{}

const cv::Mat &FlowerImage::getImageColor() const
{
    return m_image_color;
}

cv::Mat &FlowerImage::getImageColor()
{
    return m_image_color;
}

const cv::Mat &FlowerImage::getImageGrayscale() const
{
    return m_image_grayscale;
}

cv::Mat &FlowerImage::getImageGrayscale()
{
    return m_image_grayscale;
}
