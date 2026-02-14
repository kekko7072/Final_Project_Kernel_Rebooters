// Author: Francesco Vezzani

#ifndef HOG_H
#define HOG_H

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <vector>

class HOGExtractor {
    public:
        HOGExtractor(
            cv::Size winSize = cv::Size(64, 128),
            cv::Size blockSize = cv::Size(16, 16),
            cv::Size blockStride = cv::Size(8, 8),
            cv::Size cellSize = cv::Size(8, 8),
            int nbins = 9
        );

        // Extract HOG descriptors from the input image
        bool extract(const cv::Mat &image, std::vector<float> &descriptors);

        // Compute L2 distance between 2 HOG descriptor vectors
        double matchDescriptors(const std::vector<float> &descriptors1, const std::vector<float> &descriptors2) const;

    private:
        cv::HOGDescriptor hog_;
};

#endif // HOG_H
