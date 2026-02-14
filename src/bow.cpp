// Author: Francesco Vezzani

#include "bow.h"
#include <chrono>
#include <iostream>
#include <limits>

BoWExtractor::BoWExtractor(
    int nfeatures,
    int vocabularySize,
    int maxIterations,
    int attempts
) : vocabularySize_(vocabularySize),
    maxIterations_(maxIterations),
    attempts_(attempts),
    extractionTime_(0.0),
    keypointCount_(0),
    matchingTime_(0.0) {
    orb_ = cv::ORB::create(nfeatures);
}

bool BoWExtractor::computeORBDescriptors(const cv::Mat &image, cv::Mat &descriptors, int &keypointCount){
    if (image.empty()) {
        keypointCount = 0;
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
    keypointCount = static_cast<int>(keypoints.size());

    return !descriptors.empty();
}

bool BoWExtractor::buildVocabulary(const std::vector<cv::Mat> &trainImages){
    cv::Mat allDescriptors;

    for (const cv::Mat &image : trainImages) {
        cv::Mat descriptors;
        int keypointCount = 0;
        if (!computeORBDescriptors(image, descriptors, keypointCount)) {
            continue;
        }

        cv::Mat descriptors32f;
        descriptors.convertTo(descriptors32f, CV_32F);
        allDescriptors.push_back(descriptors32f);
    }

    if (vocabularySize_ <= 0 || allDescriptors.rows < vocabularySize_) {
        std::cerr << "[BOW ERROR] Not enough descriptors to build vocabulary!" << std::endl;
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
        std::cerr << "[BOW ERROR] Vocabulary not built yet!" << std::endl;
        extractionTime_ = 0.0;
        keypointCount_ = 0;
        histogram.release();
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();

    cv::Mat descriptors;
    if (!computeORBDescriptors(image, descriptors, keypointCount_)) {
        extractionTime_ = 0.0;
        histogram.release();
        return false;
    }

    histogram = computeHistogram(descriptors);

    auto end = std::chrono::high_resolution_clock::now();
    extractionTime_ = std::chrono::duration<double>(end - start).count();

    return !histogram.empty();
}

double BoWExtractor::matchDescriptors(const cv::Mat &histogram1, const cv::Mat &histogram2){
    if (histogram1.empty() || histogram2.empty() || histogram1.cols != histogram2.cols) {
        std::cerr << "[BOW ERROR] Invalid histograms for matching!" << std::endl;
        matchingTime_ = 0.0;
        return std::numeric_limits<double>::max();
    }

    auto start = std::chrono::high_resolution_clock::now();

    const double distance = cv::norm(histogram1, histogram2, cv::NORM_L2);

    auto end = std::chrono::high_resolution_clock::now();
    matchingTime_ = std::chrono::duration<double, std::milli>(end - start).count();

    return distance;
}

double BoWExtractor::getExtractionTime() const {
    return extractionTime_;
}

int BoWExtractor::getKeypointCount() const {
    return keypointCount_;
}

double BoWExtractor::getMatchingTime() const {
    return matchingTime_;
}

bool BoWExtractor::isVocabularyReady() const {
    return !vocabulary_.empty();
}
