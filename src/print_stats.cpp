// Author: Marco Carraro

#include "print_stats.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using std::cout;
using std::endl;

void printConfusionMatrix(
    const Metrics& metrics,
    const std::vector<std::string>& class_names)
{
    cout << "\nConfusion Matrix:" << endl;
    cout << std::setw(12) << " ";
    
    for (const auto& name : class_names) {
        cout << std::setw(12) << name.substr(0, 10);
    }
    cout << endl;
    
    const auto& cm = getConfusionMatrix(metrics);
    for (size_t i = 0; i < cm.size(); i++) {
        cout << std::setw(12) << class_names[i].substr(0, 10);
        for (int val : cm[i]) {
            cout << std::setw(12) << val;
        }
        cout << endl;
    }
}

static std::string formatTime(double milliseconds) {
    int minutes = static_cast<int>(milliseconds / 60000);
    int seconds = static_cast<int>((milliseconds - minutes * 60000) / 1000);
    int ms = static_cast<int>(milliseconds) % 1000;
    
    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << minutes << ":"
        << std::setw(2) << seconds << ":"
        << std::setw(3) << ms;
    return oss.str();
}

void printTimingStats(const Metrics& metrics){
    cout << "\nTiming Statistics: min:sec:ms" << endl;
    cout << "Total: " << formatTime(totalProcessingTime(metrics)) << endl;
    cout << "Mean: " << formatTime(meanProcessingTime(metrics)) << endl;
    cout << "Min: " << formatTime(minProcessingTime(metrics)) << endl;
    cout << "Max: " << formatTime(maxProcessingTime(metrics)) << endl;
}

void printPerClassAccuracy(
    const Metrics& metrics,
    const std::vector<std::string>& class_names)
{
    cout << "\nPer-Class Accuracy:" << endl;
    std::vector<double> accuracies = allClassAccuracies(metrics);
    
    for (size_t i = 0; i < accuracies.size(); i++) {
        cout << std::setw(12) << class_names[i] << ": " 
             << std::fixed << std::setprecision(2)
             << (accuracies[i] * 100.0) << "%" << endl;
    }
}

void printClassificationReport(
    const Metrics& metrics,
    const std::vector<std::string>& class_names,
    const std::string& algorithm_name)
{    
    if (!algorithm_name.empty()) {
        cout << "\n" << algorithm_name << " RESULTS " << endl;
    } else {
        cout << "\nCLASSIFICATION RESULTS " << endl;
    }

    cout << "Total samples: " << metrics.total_samples << endl;
    cout << "Overall Accuracy: " << std::fixed << std::setprecision(2)
         << (totalAccuracy(metrics) * 100.0) << "%" << endl;
    
    printTimingStats(metrics);
    printPerClassAccuracy(metrics, class_names);
    printConfusionMatrix(metrics, class_names);
}