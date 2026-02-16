// Author: Marco Carraro

#ifndef PRINT_STATS_H
#define PRINT_STATS_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include "metrics.h"


// Print confusion matrix
void printConfusionMatrix(
    const Metrics& metrics,
    const std::vector<std::string>& class_names
);

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

#endif // PRINT_STATS_H