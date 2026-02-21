// Author: Francesco Vezzani

#include "matching.h"

#include <chrono>
#include <filesystem>
#include <iostream>
#include <vector>
#include <limits>

#include <flower_type.hpp>
#include <flower_image.hpp>
#include <metrics.h>
#include <print_stats.h>
#include <hog.h>
#include <bow.h>

namespace fs = std::filesystem;

namespace
{

std::vector<const FlowerImage*> getAllTrainImages(
    const FlowerImageContainer& train_healthy_images,
    const FlowerImageContainer& train_diseased_images
)
{
    std::vector<const FlowerImage*> train_images;

    for (const FlowerImage& img : train_healthy_images.getImagesVector())
    {
        train_images.push_back(&img);
    }
    for (const FlowerImage& img : train_diseased_images.getImagesVector())
    {
        train_images.push_back(&img);
    }

    return train_images;
}

} // namespace

void hog(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy_images,
    const FlowerImageContainer& train_diseased_images,
    const std::string& output_dir
)
{
    std::cout << "\n[HOG] Simple matching" << std::endl;
    Metrics metrics = createMetrics(static_cast<int>(num_classes));
    ClassificationRecap records;

    const std::vector<const FlowerImage*> train_images =
        getAllTrainImages(train_healthy_images, train_diseased_images);
    if (test_images.empty() || train_images.empty())
    {
        std::cout << "[HOG] No images available." << std::endl;
        return;
    }

    HOGExtractor extractor;
    std::vector<std::vector<float>> train_descriptors(train_images.size());
    for (size_t i {0}; i < train_images.size(); i++)
    {
        extractor.extract(train_images[i]->getImageGrayscale(), train_descriptors[i]);
    }

    for (const FlowerImage& test_img : test_images.getImagesVector())
    {
        auto start_time = std::chrono::high_resolution_clock::now();

        std::vector<float> test_descriptor;
        extractor.extract(test_img.getImageGrayscale(), test_descriptor);
        if (test_descriptor.empty())
        {
            std::cout << "[HOG] " << test_img.name() << " -> skipped (no descriptor)" << std::endl;
            continue;
        }

        double best_distance = std::numeric_limits<double>::max();
        FlowerType predicted_type {FlowerType::NoFlower};

        for (size_t i {0}; i < train_images.size(); i++)
        {
            if (train_descriptors[i].empty())
            {
                continue;
            }

            const double distance = extractor.matchDescriptors(test_descriptor, train_descriptors[i]);
            if (distance < best_distance)
            {
                best_distance = distance;
                predicted_type = train_images[i]->flowerType();
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        const double total_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

        const int true_class = static_cast<int>(test_img.flowerType());
        const int predicted_class = static_cast<int>(predicted_type);
        addPrediction(metrics, true_class, predicted_class);
        addProcessingTime(metrics, total_time);
        records.push_back({
            test_img.name(),
            class_names[true_class],
            class_names[predicted_class]
        });

        std::cout << "[HOG] " << test_img.name()
                  << " | true " << flowerTypeToString(test_img.flowerType())
                  << " | predicted " << flowerTypeToString(predicted_type)
                  << " (distance: " << best_distance << ")"
                  << " | time: " << total_time << " ms"
                  << std::endl;
    }

    printClassificationReport(metrics, class_names, "HOG");

    fs::path output_path = fs::path(output_dir) / "hog_recap.txt";
    saveClassificationRecap(records, metrics, class_names, "HOG", output_path.string());
}

void bow(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy_images,
    const FlowerImageContainer& train_diseased_images,
    const std::string& output_dir
)
{
    std::cout << "\n[BOW] Simple matching" << std::endl;
    Metrics metrics = createMetrics(static_cast<int>(num_classes));
    ClassificationRecap records;

    const std::vector<const FlowerImage*> train_images =
        getAllTrainImages(train_healthy_images, train_diseased_images);
    if (test_images.empty() || train_images.empty())
    {
        std::cout << "[BOW] No images available." << std::endl;
        return;
    }

    BoWExtractor extractor(300, 20);
    std::vector<cv::Mat> train_gray_images;
    train_gray_images.reserve(train_images.size());
    for (const FlowerImage* train_img : train_images)
    {
        train_gray_images.push_back(train_img->getImageGrayscale());
    }

    if (!extractor.buildVocabulary(train_gray_images))
    {
        std::cout << "[BOW] Not enough descriptors to build vocabulary." << std::endl;
        return;
    }

    std::vector<cv::Mat> train_histograms(train_images.size());
    for (size_t i {0}; i < train_images.size(); i++)
    {
        extractor.extract(train_images[i]->getImageGrayscale(), train_histograms[i]);
    }

    for (const FlowerImage& test_img : test_images.getImagesVector())
    {
        auto start_time = std::chrono::high_resolution_clock::now();

        cv::Mat test_histogram;
        if (!extractor.extract(test_img.getImageGrayscale(), test_histogram))
        {
            std::cout << "[BOW] " << test_img.name() << " -> skipped (no descriptor)" << std::endl;
            continue;
        }

        double best_distance = std::numeric_limits<double>::max();
        FlowerType predicted_type {FlowerType::NoFlower};

        for (size_t i {0}; i < train_images.size(); i++)
        {
            if (train_histograms[i].empty())
            {
                continue;
            }

            const double distance = extractor.matchDescriptors(test_histogram, train_histograms[i]);
            if (distance < best_distance)
            {
                best_distance = distance;
                predicted_type = train_images[i]->flowerType();
            }
        }

        auto end_time = std::chrono::high_resolution_clock::now();
        const double total_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

        const int true_class = static_cast<int>(test_img.flowerType());
        const int predicted_class = static_cast<int>(predicted_type);
        addPrediction(metrics, true_class, predicted_class);
        addProcessingTime(metrics, total_time);
        records.push_back({
            test_img.name(),
            class_names[true_class],
            class_names[predicted_class]
        });

        std::cout << "[BOW] " << test_img.name()
                  << " | true " << flowerTypeToString(test_img.flowerType())
                  << " | predicted " << flowerTypeToString(predicted_type)
                  << " (distance: " << best_distance << ")"
                  << " | time: " << total_time << " ms"
                  << std::endl;
    }

    printClassificationReport(metrics, class_names, "BoW");

    fs::path output_path = fs::path(output_dir) / "bow_recap.txt";
    saveClassificationRecap(records, metrics, class_names, "BoW", output_path.string());
}
