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
        void extract(const cv::Mat &image, std::vector<float> &descriptors);

        // Get the time taken for the last extraction
        double getExtractionTime() const;

        // Get descriptor size from the last extraction
        int getDescriptorCount() const;

        // Compute L2 distance between 2 HOG descriptor vectors
        double matchDescriptors(const std::vector<float> &descriptors1, const std::vector<float> &descriptors2);

        // Get the time taken for the last matching operation
        double getMatchingTime() const;

    private:
        cv::HOGDescriptor hog_;
        double extractionTime_;
        int descriptorCount_;
        double matchingTime_;
};

#endif // HOG_H
