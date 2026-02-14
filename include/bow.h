// Author: Francesco Vezzani

#ifndef BOW_H
#define BOW_H

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

class BoWExtractor {
    public:
        BoWExtractor(
            int nfeatures = 300,
            int vocabularySize = 20,
            int maxIterations = 20,
            int attempts = 2
        );

        // Build visual vocabulary using train images
        bool buildVocabulary(const std::vector<cv::Mat> &trainImages);

        // Extract BoW histogram from one image
        bool extract(const cv::Mat &image, cv::Mat &histogram);

        // Get the time taken for the last extraction
        double getExtractionTime() const;

        // Get the number of keypoints from the last extraction
        int getKeypointCount() const;

        // Compute distance between 2 BoW histograms
        double matchDescriptors(const cv::Mat &histogram1, const cv::Mat &histogram2);

        // Get the time taken for the last matching operation
        double getMatchingTime() const;

        bool isVocabularyReady() const;

    private:
        bool computeORBDescriptors(const cv::Mat &image, cv::Mat &descriptors, int &keypointCount);
        cv::Mat computeHistogram(const cv::Mat &descriptors) const;

        cv::Ptr<cv::ORB> orb_;
        cv::Mat vocabulary_;
        int vocabularySize_;
        int maxIterations_;
        int attempts_;
        double extractionTime_;
        int keypointCount_;
        double matchingTime_;
};

#endif // BOW_H
