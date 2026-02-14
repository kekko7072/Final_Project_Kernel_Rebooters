// Author: Francesco Vezzani

#include "hog.h"
#include <cmath>
#include <limits>

HOGExtractor::HOGExtractor(
    cv::Size winSize,
    cv::Size blockSize,
    cv::Size blockStride,
    cv::Size cellSize,
    int nbins
) : hog_(winSize, blockSize, blockStride, cellSize, nbins) {}

bool HOGExtractor::extract(const cv::Mat &image, std::vector<float> &descriptors){
    if (image.empty()) {
        descriptors.clear();
        return false;
    }

    cv::Mat gray;
    if (image.channels() == 1) {
        gray = image;
    } else {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }

    cv::Mat resized;
    cv::resize(gray, resized, hog_.winSize);
    hog_.compute(resized, descriptors);

    return !descriptors.empty();
}

double HOGExtractor::matchDescriptors(const std::vector<float> &descriptors1, const std::vector<float> &descriptors2) const{
    if (descriptors1.empty() || descriptors2.empty() || descriptors1.size() != descriptors2.size()) {
        return std::numeric_limits<double>::max();
    }

    double sum = 0.0;
    for (size_t i = 0; i < descriptors1.size(); i++) {
        const double diff = static_cast<double>(descriptors1[i]) - static_cast<double>(descriptors2[i]);
        sum += diff * diff;
    }
    return std::sqrt(sum);
}
