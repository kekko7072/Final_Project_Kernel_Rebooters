// Author: Marco Carraro

#ifndef SURF_H
#define SURF_H

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

    private:
        cv::Ptr<cv::xfeatures2d::SURF> surf_;
        double extractionTime_;
        int keypointCount_;
};

#endif // SURF_H