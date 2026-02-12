// Author: Luca Pellegrini
#include "flower_image_container.hpp"

#include <iostream>
using std::cout;
using std::endl;

FlowerImageContainer::FlowerImageContainer()
{
    using Flt = FlowerType;
    m_map_flower_type_indices = {
        {Flt::Daisy, {}},
        {Flt::Dandelion, {}},
        {Flt::Rose, {}},
        {Flt::Sunflower, {}},
        {Flt::Tulip, {}},
        {Flt::NoFlower, {}}
    };
}

const std::vector<FlowerImage>& FlowerImageContainer::getImagesVector() const
{
    return m_vec;
}

const std::vector<FlowerImage> FlowerImageContainer::getImagesByFlowerType(const FlowerType& flower_type) const
{
    const std::vector<size_t>& img_indices_vec {m_map_flower_type_indices.at(flower_type)};
    const size_t count = img_indices_vec.size();
    std::vector<FlowerImage> temp_vec;
    for (size_t i {0}; i < count; i++)
    {
        FlowerImage temp_img {m_vec[img_indices_vec[i]]};
        temp_vec.push_back(temp_img);
    }
    return temp_vec;
}

bool FlowerImageContainer::empty() const
{
    return m_vec.empty();
}

size_t FlowerImageContainer::size() const
{
    return m_vec.size();
}

void FlowerImageContainer::push_back(const FlowerImage& img)
{
    //cout << "push_back image: " << img.name << endl;
    //cout << "        fl_type: " << static_cast<int>(img.flower_type) << endl;
    m_vec.push_back(img);
    std::vector<size_t>& indices_vec {m_map_flower_type_indices.at(img.flower_type)};
    indices_vec.push_back(m_vec.size()-1);
}

