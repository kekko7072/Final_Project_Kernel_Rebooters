// Author: Marco Carraro

#ifndef METRICS_H
#define METRICS_H

#include <vector>

// Structure to hold metrics data
struct Metrics{
    int num_classes;
    int total_samples;
    int correct_predictions;

    std::vector<std::vector<int>> confusion_matrix;
    std::vector<double> processing_times;
};

// INITIALIZATION
// Initialize metrics structure
Metrics createMetrics(int num_classes);

// Reset metrics data
void resetMetrics(Metrics &metrics);

// UPDATING
// Update metrics with a new prediction
void addPrediction(Metrics &metrics, int true_class, int predicted_class);

// Add processing time for a single prediction
void addProcessingTime(Metrics &metrics, double time);

// ACCURACY
// Calculate total accuracy
double totalAccuracy(const Metrics &metrics);

// Calculate accuracy for a specific class
double classAccuracy(const Metrics &metrics, int class_id);

// Calculate accuracy for all classes
std::vector<double> allClassAccuracies(const Metrics &metrics);

// CONFUSION MATRIX
// Get confusion matrix
const std::vector<std::vector<int>> &getConfusionMatrix(const Metrics &metrics);

// TIME PERFORMANCE
// Calculate mean processing time
double meanProcessingTime(const Metrics &metrics);

// Retrieve the minimum processing time
double minProcessingTime(const Metrics &metrics);

// Retrieve the maximum processing time
double maxProcessingTime(const Metrics &metrics);

// Calculate total processing time (sum of all individual times)
double totalProcessingTime(const Metrics &metrics);

#endif // METRICS_H