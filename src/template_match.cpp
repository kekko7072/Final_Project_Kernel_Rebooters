// Author: Luca Pellegrini
#include <template_match.hpp>

#include <iostream>
#include <filesystem>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <flower_type.hpp>  // num_classes, class_names
#include <metrics.h>
#include <print_stats.h>

namespace fs = std::filesystem;
using std::cout;
using std::cerr;
using std::endl;

using ClassificationRecord = std::array<std::string, 3>;
using ClassificationRecap = std::vector<ClassificationRecord>;

void template_match(
    const FlowerImageContainer& test_images,
    const std::vector<FlowerTemplate>& daisy_templates,
    const std::vector<FlowerTemplate>& dandelion_templates,
    const std::vector<FlowerTemplate>& rose_templates,
    const std::vector<FlowerTemplate>& sunflower_templates,
    const std::vector<FlowerTemplate>& tulip_templates,
    const fs::path output_dir,
    bool& success
)
{
    success = false;  // initial value

    // Create a Metrics object
    Metrics tm_metrics = createMetrics(num_classes);
    // Create classification records' vector
    ClassificationRecap tm_class_records;

    // for (const auto& templ : tulip_templates)
    // {
    //     const std::string templ_window {templ.name() + " template"};
    //     const std::string mask_window {templ.name() + " mask"};
    //     cv::namedWindow(templ_window, cv::WINDOW_AUTOSIZE);
    //     cv::namedWindow(mask_window, cv::WINDOW_AUTOSIZE);
    //     cv::imshow(templ_window, templ.getTemplate());
    //     cv::imshow(mask_window, templ.getMask());
    //     cv::waitKey(0);
    // }

    // Process one test image at a time
    const size_t sz {test_images.size()};
    for (size_t i {0}; i < sz; i++)
    {
        cout << "template_match: processing image [" << i+1 << "/" << sz << "] ..." << endl;
        const FlowerImage& image = test_images.at(i);
        const cv::Mat_<cv::Vec3b> img_test = image.getImageColor();
        if (img_test.empty())
        {
            cerr << "Error: template_match: empty test image" << endl;
            return;
        }

        // Start timing
        auto start_time = std::chrono::high_resolution_clock::now();

        // Resize test image to two different sizes
        cv::Mat_<cv::Vec3b> dst_1;
        cv::Mat_<cv::Vec3b> dst_2;
        cv::resize(img_test, dst_1, cv::Size(1200, 900));
        cv::resize(img_test, dst_2, cv::Size(800, 600));

        // Array to store best matches for each test images
        // For every class, store the maximum score achieved by one of the templates.
        // The highest score determines the class assigned to the test image.
        std::vector<double> class_scores(num_classes-1, 0.0);

        double score_1, score_2;
        // Daisy
        {
            score_1 = processImage(dst_1, daisy_templates);
            score_2 = processImage(dst_2, daisy_templates);
            double best_score = (score_1 > score_2) ? score_1 : score_2;
            // cout << "template_match: image " << image.name()
            //      << " class " << flowerTypeToString(FlowerType::Daisy)
            //      << " score " << best_score << endl;  // DEBUG
            class_scores.at(0) = best_score;
        }
        // Dandelion
        {
            score_1 = processImage(dst_1, dandelion_templates);
            score_2 = processImage(dst_2, dandelion_templates);
            double best_score = (score_1 > score_2) ? score_1 : score_2;
            // cout << "template_match: image " << image.name()
            //      << " class " << flowerTypeToString(FlowerType::Dandelion)
            //      << " score " << best_score << endl;  // DEBUG
            class_scores.at(1) = best_score;
        }
        // Rose
        {
            score_1 = processImage(dst_1, rose_templates);
            score_2 = processImage(dst_2, rose_templates);
            double best_score = (score_1 > score_2) ? score_1 : score_2;
            // cout << "template_match: image " << image.name()
            //      << " class " << flowerTypeToString(FlowerType::Rose)
            //      << " score " << best_score << endl;  // DEBUG
            class_scores.at(2) = best_score;
        }
        // Sunflower
        {
            score_1 = processImage(dst_1, sunflower_templates);
            score_2 = processImage(dst_2, sunflower_templates);
            double best_score = (score_1 > score_2) ? score_1 : score_2;
            // cout << "template_match: image " << image.name()
            //      << " class " << flowerTypeToString(FlowerType::Sunflower)
            //      << " score " << best_score << endl;  // DEBUG
            class_scores.at(3) = best_score;
        }
        // Tulip
        {
            score_1 = processImage(dst_1, tulip_templates);
            score_2 = processImage(dst_2, tulip_templates);
            double best_score = (score_1 > score_2) ? score_1 : score_2;
            // cout << "template_match: image " << image.name()
            //      << " class " << flowerTypeToString(FlowerType::Tulip)
            //      << " score " << best_score << endl;  // DEBUG
            class_scores.at(4) = best_score;
        }

        // for (int j {0}; j < num_classes-1; j++)
        // {
        //     double score_1, score_2;
        //     FlowerType fl_type;
        //     switch (j)
        //     {
        //     case 0:  // Daisy
        //         fl_type = FlowerType::Daisy;
        //         score_1 = processImage(img_test, daisy_templates);
        //         score_2 = processImage(img_test, daisy_templates);
        //         break;
        //     case 1:  // Dandelion
        //         fl_type = FlowerType::Dandelion;
        //         score_1 = processImage(img_test, dandelion_templates);
        //         score_2 = processImage(img_test, dandelion_templates);
        //         break;
        //     case 2:  // Rose
        //         fl_type = FlowerType::Rose;
        //         score_1 = processImage(img_test, rose_templates);
        //         score_2 = processImage(img_test, rose_templates);
        //         break;
        //     case 3:  // Sunflower
        //         fl_type = FlowerType::Sunflower;
        //         score_1 = processImage(img_test, sunflower_templates);
        //         score_2 = processImage(img_test, sunflower_templates);
        //         break;
        //     case 4:  // Tulip
        //         fl_type = FlowerType::Tulip;
        //         score_1 = processImage(img_test, tulip_templates);
        //         score_2 = processImage(img_test, tulip_templates);
        //         break;
        //     }
        //     double best_score = (score_1 > score_2) ? score_1 : score_2;
        //     cout << "template_match: image " << image.name()
        //          << " class " << flowerTypeToString(fl_type)
        //          << " score " << best_score << endl;  // DEBUG
        //     class_scores.at(j) = best_score;
        // }

        // End timing
        auto end_time = std::chrono::high_resolution_clock::now();
        double total_time = std::chrono::duration<double, std::milli>(end_time - start_time).count();

        std::vector<double>::iterator max;
        max = std::max_element(class_scores.begin(), class_scores.end());
        const int idx = std::distance(class_scores.begin(), max);
        const auto predicted_type {static_cast<FlowerType>(idx)};
        const double predicted_score {*max};

        // Update metrics
        int true_class = static_cast<int>(image.flowerType());
        int predicted_class = static_cast<int>(predicted_type);

        addPrediction(tm_metrics, true_class, predicted_class);
        addProcessingTime(tm_metrics, total_time);

        ClassificationRecord record = {
            image.name(),
            class_names[true_class],
            class_names[predicted_class]
        };
        tm_class_records.push_back(record);

        // cout << "image " << image.name() << " prediction: " << flowerTypeToString(predicted_type) << endl;
    }

    printClassificationReport(tm_metrics, class_names, "Template Match");
    // Save classification recap to file
    fs::path records_path {output_dir / "sift_recap.txt"};
    saveClassificationRecap(tm_class_records, tm_metrics, class_names, "SIFT", records_path.string());

    success = true;
}

double processImage(
    const cv::Mat_<cv::Vec3b> img_test,
    const std::vector<FlowerTemplate>& templates
)
{
    // Current best score for this class
    float score {0.0f};

    for (const auto& templ : templates)
    {
        const cv::Mat_<cv::Vec3b> img_template = templ.getTemplate();
        const cv::Mat_<uchar> img_mask = templ.getMask();
        if (img_template.empty() || img_mask.empty())
        {
            cerr << "Error: processImage: empty template of mask image" << endl;
            continue;
        }

        auto match_method = cv::TM_CCORR_NORMED;  // TM_SQDIFF or TM_CCORR_NORMED

        // const std::string image_window {"Test image"};
        // const std::string templ_window {"Template image"};
        // const std::string result_window {"TM result"};
        // cv::namedWindow(image_window, cv::WINDOW_AUTOSIZE);
        // cv::namedWindow(templ_window, cv::WINDOW_AUTOSIZE);
        // cv::namedWindow(result_window, cv::WINDOW_AUTOSIZE);
        // cv::Mat_<cv::Vec3b> img_display;
        // img_test.copyTo(img_display);

        const int result_cols = img_test.cols - img_template.cols + 1;
        const int result_rows = img_test.rows - img_template.rows + 1;
        cv::Mat_<float> result;
        result.create(result_rows, result_cols);

        cv::matchTemplate(img_test, img_template, result, match_method, img_mask);
        // Divide all elements of `result` by the mask_white_area
        // float mask_white_area = cv::countNonZero(img_mask);
        // cv::MatIterator_<float> it, end;
        // for (it = result.begin(), end = result.end(); it != end; it++)
        // {
        //     *it = *it / mask_white_area;
        // }

        // Localize min and max in result
        double minVal; double maxVal; cv::Point minLoc; cv::Point maxLoc;
        cv::Point matchLoc;
        cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc);
        if( match_method  == cv::TM_SQDIFF || match_method == cv::TM_SQDIFF_NORMED )
        {
            matchLoc = minLoc;
        }
        else
        {
            matchLoc = maxLoc;
        }
        if (maxVal > score)
        {
            score = maxVal;
        }

        // Show result
        // cv::rectangle(img_display, matchLoc, cv::Point(matchLoc.x + img_template.cols , matchLoc.y + img_template.rows), cv::Scalar::all(0), 2, 8, 0);
        // cv::rectangle(result, matchLoc, cv::Point(matchLoc.x + img_template.cols , matchLoc.y + img_template.rows), cv::Scalar::all(0), 2, 8, 0);
        // cv::imshow(image_window, img_display);
        // cv::imshow(templ_window, img_template);
        // cv::imshow(result_window, result);
        // cv::waitKey(0);
    }

    return score;
}
