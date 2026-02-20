// Author: Luca Pellegrini
#ifndef FLOWER_TEMPLATE_HPP
#define FLOWER_TEMPLATE_HPP

#include <string>
#include <opencv2/core.hpp>
#include <flower_type.hpp>
#include <flower_image.hpp>

/**
 * @brief A FlowerTemplate object represents a template (color image) and its associated mask
 */
class FlowerTemplate : public FlowerImage
{
public:
    FlowerTemplate() = default;
    FlowerTemplate(std::string name, FlowerType fl_type, bool healthy, int img_type,
                   cv::Mat_<cv::Vec3b>& img_templ, cv::Mat_<uchar>& img_mask);

    /**
     * @brief Returns the Template associated with the image
     */
    const cv::Mat_<cv::Vec3b>& getTemplate() const;
    cv::Mat_<cv::Vec3b>& getTemplate();

    /**
     * @brief Returns the Grayscale Mask associated with the image
     */
    const cv::Mat_<uchar>& getMask() const;
    cv::Mat_<uchar>& getMask();
};

#endif // FLOWER_TEMPLATE_HPP
