// Author: Luca Pellegrini
#include "flower_template.hpp"

FlowerTemplate::FlowerTemplate(
    std::string name, FlowerType fl_type, bool healthy, int img_type,
    cv::Mat_<cv::Vec3b>& img_templ, cv::Mat_<uchar>& img_mask) :
    FlowerImage{name, fl_type, healthy, img_type, img_templ, img_mask}
{}

const cv::Mat_<cv::Vec3b>& FlowerTemplate::getTemplate() const
{
    return m_image_color;
}

cv::Mat_<cv::Vec3b>& FlowerTemplate::getTemplate()
{
    return m_image_color;
}

const cv::Mat_<uchar>& FlowerTemplate::getMask() const
{
    return m_image_grayscale;
}

cv::Mat_<uchar>& FlowerTemplate::getMask()
{
    return m_image_grayscale;
}
