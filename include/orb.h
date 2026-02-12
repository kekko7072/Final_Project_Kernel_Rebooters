// Author: Marco Carraro

#ifndef ORB_H
#define ORB_H

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

class ORBExtractor {
    public:
        ORBExtractor(                       // Default parameters based on OpenCV documentation
            int nfeatures = 500,            // The maximum number of features to retain
            float scaleFactor = 1.2f,       // Pyramid decimation ratio, greater than 1. scaleFactor==2 means the classical pyramid, where each next level has 4x less pixels than the previous, but such a big scale factor will degrade feature matching scores dramatically
            int nlevels = 8,                // The number of pyramid levels
            int edgeThreshold = 31,         // This is size of the border where the features are not detected. It should roughly match the patchSize parameter
            int firstLevel = 0,             // The level of pyramid to put source image to. Previous layers are filled with upscaled source image
            int WTA_K = 2,                  // The number of points that produce each element of the oriented BRIEF descriptor. The default value 2 means the BRIEF where we take a random point pair and compare their brightnesses, so we get 0/1 response. Other possible values are 3 and 4. 3 means that we take 3 random points. 4 means we take 4 random points to compute each bin
            int patchSize = 31              // size of the patch used by the oriented BRIEF descriptor
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
        cv::Ptr<cv::ORB> orb_;
        cv::Ptr<cv::BFMatcher> matcher_;
        double extractionTime_;
        int keypointCount_;
        double matchingTime_;
};

#endif // ORB_H