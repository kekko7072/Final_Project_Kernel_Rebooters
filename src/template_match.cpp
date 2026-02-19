// Author: Luca Pellegrini
#include <template_match.hpp>

extern const std::vector<std::string> class_names;  // defined in header
extern const size_t num_classes;                    // "flower_type.hpp"

#include <iostream>
#include <filesystem>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <metrics.h>
#include <print_stats.h>

namespace fs = std::filesystem;
using std::cout;
using std::cerr;
using std::endl;

void processImage();

void template_match(
    const FlowerImageContainer& test_images,
    const std::vector<FlowerTemplate>& daisy_templates,
    const std::vector<FlowerTemplate>& dandelion_templates,
    const std::vector<FlowerTemplate>& rose_templates,
    const std::vector<FlowerTemplate>& sunflower_templates,
    const std::vector<FlowerTemplate>& tulip_templates,
    bool& success
)
{
    success = false;  // initial value

    // Create a Metrics object
    Metrics tm_metrics = createMetrics(num_classes);
    // Create classification records' vector
    std::vector< std::array< std::string, 3 > > tm_class_records;

    // Process one test image at a time

    processImage();

    // printClassificationReport(tm_metrics, class_names, "Template Match");
    success = true;
}

void processImage()
{
    // Dandelion
    // template:
    const fs::path img_template_path {"/Data/Git/Computer_Vision/FINAL_PROJECT/Final_project_proposal/train_healthy_photos/dandelion/templates/t_dandelion_healthy_5.png"};
    const fs::path img_mask_path {"/Data/Git/Computer_Vision/FINAL_PROJECT/Final_project_proposal/train_healthy_photos/dandelion/templates/m_dandelion_healthy_5.png"};
    const fs::path img_test_path {"/Data/Git/Computer_Vision/FINAL_PROJECT/Final_project_proposal/test_photos/dandelion/dandelion_test_5.jpg"};

    cv::Mat_<cv::Vec3b> img_test = cv::imread(img_test_path.c_str(), cv::IMREAD_COLOR);
    cv::Mat_<cv::Vec3b> img_template = cv::imread(img_template_path.c_str(), cv::IMREAD_COLOR);
    cv::Mat_<uchar> img_mask = cv::imread(img_mask_path.c_str(), cv::IMREAD_GRAYSCALE);

    if (img_test.empty() || img_template.empty() || img_mask.empty())
    {
        cerr << "Cannot read one of the images!" << endl;
        return;
    }

    auto match_method = cv::TM_CCORR_NORMED;  // TM_SQDIFF or TM_CCORR_NORMED

    const std::string image_window {"Test image"};
    const std::string templ_window {"Template image"};
    const std::string result_window {"TM result"};
    cv::namedWindow(image_window, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(templ_window, cv::WINDOW_AUTOSIZE);
    cv::namedWindow(result_window, cv::WINDOW_AUTOSIZE);

    cv::Mat_<cv::Vec3b> img_display;
    img_test.copyTo(img_display);

    const int result_cols = img_test.cols - img_template.cols + 1;
    const int result_rows = img_test.rows - img_template.rows + 1;
    cv::Mat_<float> result;
    result.create(result_rows, result_cols);

    cv::matchTemplate(img_test, img_template, result, match_method);
    cv::normalize(result, result, 0, 1, cv::NORM_MINMAX, -1);

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

    // Show result
    cv::rectangle(img_display, matchLoc, cv::Point(matchLoc.x + img_template.cols , matchLoc.y + img_template.rows), cv::Scalar::all(0), 2, 8, 0);
    cv::rectangle(result, matchLoc, cv::Point(matchLoc.x + img_template.cols , matchLoc.y + img_template.rows), cv::Scalar::all(0), 2, 8, 0);
    cv::imshow(image_window, img_display);
    cv::imshow(templ_window, img_template);
    cv::imshow(result_window, result);

    cv::waitKey(0);
}
