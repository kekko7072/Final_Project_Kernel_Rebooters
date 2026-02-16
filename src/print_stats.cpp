// Author: Marco Carraro

#include "print_stats.h"
#include <iostream>
#include <iomanip>
#include <fstream>

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

void printTimingStats(const Metrics& metrics){
    cout << "\nTiming Statistics:" << endl;
    cout << "Total: " << std::fixed << std::setprecision(2) << (totalProcessingTime(metrics) / 1000.0) << " s (" 
                                                            << (totalProcessingTime(metrics) / 60000.0) << " minutes)" << endl;
    cout << "Mean: " << meanProcessingTime(metrics) << " ms" << endl;
    cout << "Min: " << minProcessingTime(metrics) << " ms" << endl;
    cout << "Max: " << maxProcessingTime(metrics) << " ms" << endl;
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