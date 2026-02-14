// Author: Francesco Vezzani

#include "bow.h"
#include <limits>

BoWExtractor::BoWExtractor(
    int nfeatures,
    int vocabularySize,
    int maxIterations,
    int attempts
) : vocabularySize_(vocabularySize),
    maxIterations_(maxIterations),
    attempts_(attempts),
    orb_(cv::ORB::create(nfeatures)) {}

bool BoWExtractor::computeORBDescriptors(const cv::Mat &image, cv::Mat &descriptors) const{
    if (image.empty()) {
        return false;
    }

    cv::Mat gray;
    if (image.channels() == 1) {
        gray = image;
    } else {
        cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    }

    std::vector<cv::KeyPoint> keypoints;
    orb_->detectAndCompute(gray, cv::noArray(), keypoints, descriptors);

    return !descriptors.empty();
}

bool BoWExtractor::buildVocabulary(const std::vector<cv::Mat> &trainImages){
    cv::Mat allDescriptors;

    for (const cv::Mat &image : trainImages) {
        cv::Mat descriptors;
        if (!computeORBDescriptors(image, descriptors)) {
            continue;
        }

        cv::Mat descriptors32f;
        descriptors.convertTo(descriptors32f, CV_32F);
        allDescriptors.push_back(descriptors32f);
    }

    if (vocabularySize_ <= 0 || allDescriptors.rows < vocabularySize_) {
        vocabulary_.release();
        return false;
    }

    cv::Mat labels;
    cv::kmeans(
        allDescriptors,
        vocabularySize_,
        labels,
        cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, maxIterations_, 0.1),
        attempts_,
        cv::KMEANS_PP_CENTERS,
        vocabulary_
    );

    return !vocabulary_.empty();
}

cv::Mat BoWExtractor::computeHistogram(const cv::Mat &descriptors) const{
    cv::Mat histogram = cv::Mat::zeros(1, vocabulary_.rows, CV_32F);
    if (descriptors.empty() || vocabulary_.empty()) {
        return histogram;
    }

    cv::Mat descriptors32f;
    descriptors.convertTo(descriptors32f, CV_32F);

    for (int i = 0; i < descriptors32f.rows; i++) {
        double bestDistance = std::numeric_limits<double>::max();
        int bestWord = -1;

        for (int j = 0; j < vocabulary_.rows; j++) {
            const double distance = cv::norm(descriptors32f.row(i), vocabulary_.row(j), cv::NORM_L2);
            if (distance < bestDistance) {
                bestDistance = distance;
                bestWord = j;
            }
        }

        if (bestWord >= 0) {
            histogram.at<float>(0, bestWord) += 1.0f;
        }
    }

    const float sum = static_cast<float>(cv::sum(histogram)[0]);
    if (sum > 0.0f) {
        histogram /= sum;
    }

    return histogram;
}

bool BoWExtractor::extract(const cv::Mat &image, cv::Mat &histogram){
    if (vocabulary_.empty()) {
        histogram.release();
        return false;
    }

    cv::Mat descriptors;
    if (!computeORBDescriptors(image, descriptors)) {
        histogram.release();
        return false;
    }

    histogram = computeHistogram(descriptors);

    return !histogram.empty();
}

double BoWExtractor::matchDescriptors(const cv::Mat &histogram1, const cv::Mat &histogram2) const{
    if (histogram1.empty() || histogram2.empty() ||
        histogram1.rows != histogram2.rows || histogram1.cols != histogram2.cols) {
        return std::numeric_limits<double>::max();
    }

    return cv::norm(histogram1, histogram2, cv::NORM_L2);
}
