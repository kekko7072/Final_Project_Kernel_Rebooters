// Author: Marco Carraro

#ifndef PRINT_STATS_H
#define PRINT_STATS_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <array>
#include <map>
#include "metrics.h"

using ClassificationRecord = std::array<std::string, 3>;
using ClassificationRecap = std::vector<ClassificationRecord>;

// Print confusion matrix
void printConfusionMatrix(
    const Metrics& metrics,
    const std::vector<std::string>& class_names
);

// Helper function to format time in mm:ss:ms
static std::string formatTime(double milliseconds);

// Print classification statistics
void printClassificationReport(
    const Metrics& metrics,
    const std::vector<std::string>& class_names,
    const std::string& algorithm_name = ""
);

// Print timing statistics
void printTimingStats(const Metrics& metrics);

// Print accuracy per class
void printPerClassAccuracy(
    const Metrics& metrics,
    const std::vector<std::string>& class_names
);

// Print classification recap for a single algorithm
void saveClassificationRecap(
    const ClassificationRecap& records,
    const Metrics& metrics,
    const std::vector<std::string>& class_names,
    const std::string& algorithm_name,
    const std::string& output_filename
);

#endif // PRINT_STATS_H