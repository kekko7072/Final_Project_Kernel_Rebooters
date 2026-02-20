// Author: Luca Pellegrini
#include <preprocessing.hpp>

#include <filesystem>
#include <iostream>
#include <set>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <flower_image_container.hpp>

namespace fs = std::filesystem;
using std::cout;
using std::cerr;
using std::endl;

bool loadImages(
    const fs::path data_path,
    FlowerImageContainer& test_imgs,
    FlowerImageContainer& train_healthy_imgs,
    FlowerImageContainer& train_diseased_imgs
)
{
    const fs::path test_path {data_path / "test_photos"};
    const fs::path train_healthy_path {data_path / "train_healthy_photos"};
    const fs::path train_diseased_path {data_path / "train_diseased_photos"};

    // Load test images
    bool ok_1 = loadImagesFromDataset(test_path, true, 1, test_imgs);
    bool ok_2 = loadImagesFromDataset(train_healthy_path, true, 0, train_healthy_imgs);
    bool ok_3 = loadImagesFromDataset(train_diseased_path, false, 0, train_diseased_imgs);

    return (ok_1 && ok_2 && ok_3);
}

bool loadImagesFromDataset(
    const fs::path dir_path,
    const bool healthy,
    const int img_type,
    FlowerImageContainer& imgs
)
{
    using FlTp = FlowerType;
    FlTp fl_type;
    fs::directory_iterator fl_type_iterator {dir_path, fs::directory_options::skip_permission_denied};
    for (const auto& dir_entry : fl_type_iterator)
    {
        if (fs::is_directory(dir_entry))
        {
            const std::string s {dir_entry.path().filename()};
            cout << dir_entry.path() << endl;  // DEBUG
            if (s == "daisy")
                fl_type = FlTp::Daisy;
            else if (s == "dandelion")
                fl_type = FlTp::Dandelion;
            else if (s == "roses")
                fl_type = FlTp::Rose;
            else if (s == "sunflowers")
                fl_type = FlTp::Sunflower;
            else if (s == "tulips")
                fl_type = FlTp::Tulip;
            else
                fl_type = FlTp::NoFlower;

            fs::directory_iterator img_iterator {dir_entry.path(), fs::directory_options::skip_permission_denied};
            for (const auto& img_entry : img_iterator)
            {
                const auto img_path {img_entry.path()};
                if (fs::is_regular_file(img_entry) && isImage(img_path))
                {
                    const std::string img_path_str {img_path.string()};
                    const std::string img_name {img_path.stem().string()};
                    //cout << "Loading image: " << img_path_str << endl;  // DEBUG
                    cv::Mat_<cv::Vec3b> img_color = cv::imread(img_path_str, cv::IMREAD_COLOR);
                    cv::Mat_<uchar> img_gray = cv::imread(img_path_str, cv::IMREAD_GRAYSCALE);
                    if (img_color.empty() || img_gray.empty())
                    {
                        cerr << "Error loading image: " << img_path_str << endl;
                        return false;
                    }
                    FlowerImage img {img_name, fl_type, healthy, img_type, img_color, img_gray};
                    imgs.push_back(img);
                }
            }
        }
    }
    return true;
}

bool loadTemplates(
    const fs::path data_path,
    std::vector<FlowerTemplate>& daisy_templates,
    std::vector<FlowerTemplate>& dandelion_templates,
    std::vector<FlowerTemplate>& rose_templates,
    std::vector<FlowerTemplate>& sunflower_templates,
    std::vector<FlowerTemplate>& tulip_templates
)
{
    using FlTp = FlowerType;
    const fs::path train_path {data_path / "train_healthy_photos"};
    // Daisy
    bool ok_1 = loadTemplateFromDataset(
        train_path / "daisy" / "templates", FlTp::Daisy, true, daisy_templates);
    // Dandelion
    bool ok_2 = loadTemplateFromDataset(
        train_path / "dandelion" / "templates", FlTp::Dandelion, true, dandelion_templates);
    // Rose
    bool ok_3 = loadTemplateFromDataset(
        train_path / "roses" / "templates", FlTp::Rose, true, rose_templates);
    // Sunflower
    bool ok_4 = loadTemplateFromDataset(
        train_path / "sunflowers" / "templates", FlTp::Sunflower, true, sunflower_templates);
    // Tulip
    bool ok_5 = loadTemplateFromDataset(
        train_path / "tulips" / "templates", FlTp::Tulip, true, tulip_templates);

    return (ok_1 && ok_2 && ok_3 && ok_4 && ok_5);
}

bool loadTemplateFromDataset(
    const fs::path template_dir_path,
    const FlowerType fl_type,
    const bool healthy,
    std::vector<FlowerTemplate>& templates
)
{
    cout << "Loading templates: class " << flowerTypeToString(fl_type)
        << "\n    source dir: " << template_dir_path.string() << endl;  // DEBUG
    const size_t num_templ_per_class {5};
    std::array<std::string, 3*num_templ_per_class> array;  // path of template and mask images
    fs::directory_iterator img_iterator {template_dir_path, fs::directory_options::skip_permission_denied};
    for (const auto& img_entry : img_iterator)
    {
        const auto img_path {img_entry.path()};
        if (fs::is_regular_file(img_path) && isImage(img_path))  // ignore GIMP .xcf files
        {
            const std::string img_name_short {img_path.stem().string()};
            // e.g. from ".../templates/m_tulip_healthy_5.png" --> "m_tulip_healthy_5"
            // cout << img_name_short << endl;  // DEBUG
            const std::string img_path_str {img_path.string()};
            const char n {img_name_short.back()};
            size_t idx;
            switch (n)
            {
            case '1':
                idx = 0; break;
            case '2':
                idx = 1; break;
            case '3':
                idx = 2; break;
            case '4':
                idx = 3; break;
            case '5':
                idx = 4; break;
            default:
                cerr << "Unrecognized char: " << n << endl;
            }
            array.at(3*idx) = img_name_short.substr(2);
            if (img_name_short.compare(0, 2, "t_") == 0)
            {
                // cout << "array index: " <<3*idx << endl;
                array.at(3*idx + 1) = img_path_str;
            }
            else
            {
                // cout << "array index: " << 3*idx + 1 << endl;
                array.at(3*idx + 2) = img_path_str;
            }
        }
    }

    size_t count_loaded {0};
    for (size_t i {0}; i < num_templ_per_class; i++)
    {
        if (array[3*i].empty() || array[3*i+1].empty() || array[3*i+2].empty())
        {
            cerr << "Missing template " << i << endl;
            continue;
        }
        // cout << array[3*i] << endl;  // DEBUG
        cv::Mat_<cv::Vec3b> img_templ = cv::imread(array.at(3*i + 1), cv::IMREAD_COLOR);
        cv::Mat_<uchar> img_mask = cv::imread(array.at(3*i + 2), cv::IMREAD_GRAYSCALE);
        if (img_templ.empty() || img_mask.empty())
        {
            cerr << "Error loading template or mask" << endl;
            continue;
        }
        // Resize all template and mask images to the same size
        auto sz {cv::Size(400, 300)};
        cv::Mat_<cv::Vec3b> dst_templ;
        cv::Mat_<uchar> dst_mask;
        cv::resize(img_templ, dst_templ, sz);
        cv::resize(img_mask, dst_mask, sz);
        FlowerTemplate templ {array.at(3*i), fl_type, healthy, 0, dst_templ, dst_mask};
        templates.push_back(templ);
        count_loaded++;
    }

    return (count_loaded == num_templ_per_class);
}

bool isImage(const fs::path& file_path)
{
    // Recognized extensions: JPG, JPEG, PNG, WEBP
    static const std::set<std::string> known_extensions {{"jpg", "JPG", "jpeg", "JPEG", "png", "PNG", "webp", "WEBP"}};
    if (!file_path.has_extension())
    {
        return false;
    }
    const fs::path file_ext {file_path.extension()};
    const std::string ext_str {file_ext.string().substr(1)};  // strip away the '.' character
    if (auto search = known_extensions.find(ext_str); search != known_extensions.end())
    {
        // Found
        return true;
    }
    else
    {
        return false;
    }
}
