// Author: Marco Carraro

#include "metrics.h"

// Initialize metrics structure
Metrics createMetrics(int num_classes){
    Metrics metrics;
    metrics.num_classes = num_classes;
    metrics.total_samples = 0;
    metrics.correct_predictions = 0;

    metrics.confusion_matrix.resize(
        num_classes,
        std::vector<int>(num_classes, 0)
    );

    return metrics;
}

// Reset metrics data
void resetMetrics(Metrics &metrics){
    metrics.total_samples = 0;
    metrics.correct_predictions = 0;

    for (auto &row : metrics.confusion_matrix) {
        std::fill(row.begin(), row.end(), 0);
    }

    metrics.processing_times.clear();
}

// UPDATING
// Update metrics with a new prediction
void addPrediction(Metrics &metrics, int true_class, int predicted_class){
    metrics.total_samples++;
    if (true_class == predicted_class) {
        metrics.correct_predictions++;
    }
    metrics.confusion_matrix[true_class][predicted_class]++;
}

// Add processing time for a single prediction
void addProcessingTime(Metrics &metrics, double time){
    metrics.processing_times.push_back(time);
}

// ACCURACY
// Calculate total accuracy
double totalAccuracy(const Metrics &metrics){
    if (metrics.total_samples == 0) 
        return 0.0;

    return static_cast<double>(metrics.correct_predictions) / metrics.total_samples;
}

// Calculate accuracy for a specific class
double classAccuracy(const Metrics &metrics, int class_id){
    if (class_id < 0 || class_id >= metrics.num_classes)    // To change when enum will be implemented
        return 0.0;

    int total_class_samples = 0;

    for (int i = 0; i < metrics.num_classes; i++) {
        total_class_samples += metrics.confusion_matrix[class_id][i];
    }

    if (total_class_samples == 0) 
        return 0.0;

    return static_cast<double>(metrics.confusion_matrix[class_id][class_id]) / total_class_samples;
}

// Calculate accuracy for all classes
std::vector<double> allClassAccuracies(const Metrics &metrics){
    std::vector<double> accuracies;

    for (int i = 0; i < metrics.num_classes; i++){
        accuracies.push_back(classAccuracy(metrics, i));
    }

    return accuracies;
}

// CONFUSION MATRIX
// Get confusion matrix
const std::vector<std::vector<int>> &getConfusionMatrix(const Metrics &metrics){
    return metrics.confusion_matrix;
}

// TIME PERFORMANCE
// Calculate mean processing time
double meanProcessingTime(const Metrics &metrics){
    if (metrics.processing_times.empty()) 
        return 0.0;

    double sum = 0.0;
    for (double time : metrics.processing_times){
        sum += time;
    }

    return sum / metrics.processing_times.size();
}

// Retrieve the minimum processing time
double minProcessingTime(const Metrics &metrics){
    if (metrics.processing_times.empty()) 
        return 0.0;

    double min_time = metrics.processing_times[0];
    for (double time : metrics.processing_times){
        if (time < min_time) {
            min_time = time;
        }
    }

    return min_time;
}

// Retrieve the maximum processing time
double maxProcessingTime(const Metrics &metrics){
    if (metrics.processing_times.empty()) 
        return 0.0;

    double max_time = metrics.processing_times[0];
    for (double time : metrics.processing_times){
        if (time > max_time) {
            max_time = time;
        }
    }
    
    return max_time;
}