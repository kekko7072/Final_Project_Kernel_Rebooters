// Author: Luca Pellegrini
#include <preprocessing.hpp>

#include <filesystem>
#include <iostream>
#include <set>
#include <opencv2/imgcodecs.hpp>
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
    bool success1 = loadImagesFromDataset(test_path, true, 1, test_imgs);
    bool success2 = loadImagesFromDataset(train_healthy_path, true, 0, train_healthy_imgs);
    bool success3 = loadImagesFromDataset(train_diseased_path, false, 0, train_diseased_imgs);
    if (!success1 || !success2 || !success3)
    {
        return false;
    }

    return true;
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
            cout << dir_entry.path() << endl;
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
                    const std::string img_name {img_path.filename().string()};
                    //cout << "Loading image: " << img_path_str << endl;
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
