// Author: Francesco Vezzani

#include "matching.h"

#include <iostream>
#include <vector>
#include <limits>

#include <flower_type.hpp>
#include <flower_image.hpp>
#include <hog.h>
#include <bow.h>

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
    const FlowerImageContainer& train_diseased_images
)
{
    std::cout << "\n[HOG] Simple matching" << std::endl;

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
        std::vector<float> test_descriptor;
        extractor.extract(test_img.getImageGrayscale(), test_descriptor);
        if (test_descriptor.empty())
        {
            std::cout << "[HOG] " << test_img.name() << " -> skipped (no descriptor)" << std::endl;
            continue;
        }

        double best_distance = std::numeric_limits<double>::max();
        const FlowerImage* best_train_img {nullptr};

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
                best_train_img = train_images[i];
            }
        }

        if (best_train_img == nullptr)
        {
            std::cout << "[HOG] " << test_img.name() << " -> no match" << std::endl;
            continue;
        }

        std::cout << "[HOG] " << test_img.name()
                  << " -> predicted " << flowerTypeToString(best_train_img->flowerType())
                  << " (distance: " << best_distance << ")"
                  << std::endl;
    }
}

void bow(
    const FlowerImageContainer& test_images,
    const FlowerImageContainer& train_healthy_images,
    const FlowerImageContainer& train_diseased_images
)
{
    std::cout << "\n[BOW] Simple matching" << std::endl;

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
        cv::Mat test_histogram;
        if (!extractor.extract(test_img.getImageGrayscale(), test_histogram))
        {
            std::cout << "[BOW] " << test_img.name() << " -> skipped (no descriptor)" << std::endl;
            continue;
        }

        double best_distance = std::numeric_limits<double>::max();
        const FlowerImage* best_train_img {nullptr};

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
                best_train_img = train_images[i];
            }
        }

        if (best_train_img == nullptr)
        {
            std::cout << "[BOW] " << test_img.name() << " -> no match" << std::endl;
            continue;
        }

        std::cout << "[BOW] " << test_img.name()
                  << " -> predicted " << flowerTypeToString(best_train_img->flowerType())
                  << " (distance: " << best_distance << ")"
                  << std::endl;
    }
}
