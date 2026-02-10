// Author: Marco Carraro

#ifndef SIFT_H
#define SIFT_H

#include <opencv2/opencv.hpp>
#include <opencv2/features2d.hpp>
#include <vector>

class SIFTExtractor {
    public:
        SIFTExtractor(                          // Default parameters based on OpenCV documentation
            int nfeatures = 0,                  // The number of best features to retain. The features are ranked by their scores (0 means default number of features)
            int nOctaveLayers = 3,              // The number of layers in each octave. 3 is the value used in D. Lowe paper. The number of octaves is computed automatically from the image resolution
            double contrastThreshold = 0.04,    // The contrast threshold used to filter out weak features in semi-uniform (low-contrast) regions. The larger the threshold, the less features are produced by the detector
            double edgeThreshold = 10,          // The threshold used to filter out edge-like features
            double sigma = 1.6                  // The sigma of the Gaussian applied to the input image at the octave #0. If your image is captured with a weak camera with soft lenses, you might want to reduce the number
        );

        // Extract keypoints and descriptors from the input image
        void extract(const cv::Mat &image, std::vector<cv::KeyPoint> &keypoints, cv::Mat &descriptors);

        // Get the time taken for the last extraction
        double getExtractionTime() const;

        // Get the number of keypoints detected in the last extraction
        int getKeypointCount() const;

    private:
        // OpenCV SIFT feature extractor
        cv::Ptr<cv::SIFT> sift_;
        double extractionTime_;
        int keypointCount_;
};

#endif // SIFT_H