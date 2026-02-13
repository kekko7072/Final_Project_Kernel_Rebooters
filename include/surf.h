// Author: Marco Carraro

#ifndef SURF_H
#define SURF_H

#ifdef ENABLE_SURF

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <vector>

class SURFExtractor {
    public:
        SURFExtractor(                        // Default parameters based on OpenCV documentation
            double hessianThreshold = 100.0,  // Threshold for hessian keypoint detector
            int nOctaves = 4,                 // Number of pyramid octaves the keypoint detector will use
            int nOctaveLayers = 3,            // Number of octave layers within each octave
            bool extended = false,            // Extended descriptor flag (true = 128, false = 64)
            bool upright = false              // Up-right or rotated features flag (true = do not compute orientation of features, false = compute orientation)
        );

        // Extract keypoints and descriptors from the input image
        void extract(const cv::Mat &image, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);

        // Get the time taken for the last extraction
        double getExtractionTime() const;

        // Get the number of keypoints detected in the last extraction
        int getKeypointCount() const;

        // Match descriptors between two sets of keypoints
        int matchDescriptors(const cv::Mat &descriptors1, const cv::Mat &descriptors2, std::vector<cv::DMatch> &matches);

        // Filter matches keeping only those with a distance less than a specified threshold
        std::vector<cv::DMatch> filterMatches(const std::vector<cv::DMatch> &matches, double threshold = 2.0);

        // Match descriptors and filter matches in one step
        int matchAndFilter(const cv::Mat &descriptors1, const cv::Mat &descriptors2, std::vector<cv::DMatch> &goodMatches, double threshold = 2.0);

        // Get the time taken for the last matching operation
        double getMatchingTime() const;

    private:
        cv::Ptr<cv::xfeatures2d::SURF> surf_;
        cv::Ptr<cv::BFMatcher> matcher_;
        double extractionTime_;
        int keypointCount_;
        double matchingTime_;
};

#endif // ENABLE_SURF
#endif // SURF_H
