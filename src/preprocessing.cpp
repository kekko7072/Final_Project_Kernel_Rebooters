// Author: Luca Pellegrini
#include <preprocessing.hpp>

#include <filesystem>
#include <flower_image_container.hpp>

namespace fs = std::filesystem;

bool loadImages(
    const fs::path& data_path,
    FlowerImageContainer& test_imgs,
    FlowerImageContainer& train_healthy_imgs,
    FlowerImageContainer& train_diseased_imgs
)
{
    return true;
}
