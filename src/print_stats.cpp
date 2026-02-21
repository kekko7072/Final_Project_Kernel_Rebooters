// Author: Marco Carraro

#include "print_stats.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>

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

void saveClassificationRecap(
    const ClassificationRecap& records,
    const Metrics& metrics,
    const std::vector<std::string>& class_names,
    const std::string& algorithm_name,
    const std::string& output_filename)
{
    std::ofstream outfile(output_filename);
    
    if (!outfile.is_open()) {
        std::cerr << "[ERROR] Could not open file: " << output_filename << std::endl;
        return;
    }
    
    // Header
    outfile << algorithm_name << " - Classification Results\n" << std::endl;
    
    // Compute max filename length for formatting
    size_t max_filename_len = 20;
    for (const auto& record : records) {
        max_filename_len = std::max(max_filename_len, record[0].length());
    }
    
    // Table header
    outfile << std::left
            << std::setw(max_filename_len + 2) << "Filename"
            << std::setw(18) << "True Class"
            << std::setw(18) << "Predicted Class" << std::endl;
    outfile << std::string(max_filename_len + 41, '-') << std::endl;
    
    // Table rows
    for (const auto& record : records) {
        outfile << std::setw(max_filename_len + 2) << record[0]
                << std::setw(18) << record[1]
                << std::setw(18) << record[2];
        
        // Indicate correctness
        if (record[1] == record[2]) {
            outfile << " V";
        } else {
            outfile << " X";
        }
        outfile << std::endl;
    }
    
    // Summary
    outfile << std::string(max_filename_len + 41, '-') << std::endl;
    outfile << std::endl;
    outfile << "Summary:" << std::endl;
    outfile << "Total samples:     " << metrics.total_samples << std::endl;
    outfile << "Correct:           " << metrics.correct_predictions << std::endl;
    outfile << "Overall Accuracy:  " << std::fixed << std::setprecision(2)
                                     << totalAccuracy(metrics) * 100.0 << "%" << std::endl;
    outfile << "Total time:        " << totalProcessingTime(metrics) << std::endl;
    
    outfile << std::endl;
    outfile << "Processing Time Statistics (ms)" << std::endl;
    outfile << "Mean time:         " << meanProcessingTime(metrics) << std::endl;
    outfile << "Min time:          " << minProcessingTime(metrics) << std::endl;
    outfile << "Max time:          " << maxProcessingTime(metrics) << std::endl;
    outfile << "Total time:        " << totalProcessingTime(metrics) << std::endl;
    
    outfile << std::endl;
    outfile << "Per-class Accuracy:" << std::endl;
    std::vector<double> accuracies = allClassAccuracies(metrics);

    for (size_t i = 0; i < accuracies.size(); ++i) {
        std::string label = class_names[i] + ":";
        outfile << std::setw(19) << label
                << std::fixed << std::setprecision(2)
                << (accuracies[i] * 100.0) << "%" << std::endl;
    }

    outfile << std::endl;
    outfile << "Confusion Matrix:" << std::endl;
    outfile << std::setw(12) << " ";
    
    for (const auto& name : class_names) {
        outfile << std::setw(12) << name.substr(0, 10);
    }
    outfile << endl;
    
    const auto& cm = getConfusionMatrix(metrics);
    for (size_t i = 0; i < cm.size(); i++) {
        outfile << std::setw(12) << class_names[i].substr(0, 10);
        for (int val : cm[i]) {
            outfile << std::setw(12) << val;
        }
        outfile << endl;
    }
        
    outfile.close();
    
    cout << "\n[" << algorithm_name << "] Classification recap saved to: " 
         << output_filename << endl;
}