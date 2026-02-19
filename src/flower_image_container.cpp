// Author: Luca Pellegrini
#include "flower_image_container.hpp"

#include <iostream>
using std::cout;
using std::endl;

FlowerImageContainer::FlowerImageContainer()
{
    using FlTp = FlowerType;
    m_map = {
        {FlTp::Daisy, {}},
        {FlTp::Dandelion, {}},
        {FlTp::Rose, {}},
        {FlTp::Sunflower, {}},
        {FlTp::Tulip, {}},
        {FlTp::NoFlower, {}}
    };
}

const std::vector<FlowerImage>& FlowerImageContainer::getImagesVector() const
{
    return m_vec;
}

const std::map<FlowerType, std::vector<size_t>>& FlowerImageContainer::getMap() const
{
    return m_map;
}

const std::vector<FlowerImage> FlowerImageContainer::getImagesByFlowerType(const FlowerType& flower_type) const
{
    const std::vector<size_t>& indices_vec {m_map.at(flower_type)};
    const size_t count = indices_vec.size();
    std::vector<FlowerImage> temp_vec(count);
    for (size_t i {0}; i < count; i++)
    {
        FlowerImage temp_img {m_vec[indices_vec[i]]};
        temp_vec[i] = temp_img;
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

FlowerImage& FlowerImageContainer::at(const size_t i)
{
    return m_vec.at(i);
}

const FlowerImage& FlowerImageContainer::at(const size_t i) const
{
    return m_vec.at(i);
}

void FlowerImageContainer::push_back(const FlowerImage& img)
{
    //cout << "push_back image: " << img.name << endl;
    //cout << "        fl_type: " << static_cast<int>(img.flowerType()) << endl;
    m_vec.push_back(img);
    std::vector<size_t>& indices_vec {m_map.at(img.flowerType())};
    indices_vec.push_back(m_vec.size()-1);
}

void FlowerImageContainer::combineContainers(
    const FlowerImageContainer& first,
    const FlowerImageContainer& second,
    FlowerImageContainer& out
)
{
    using FlTp = FlowerType;
    using MapTp = std::map<FlTp, std::vector<size_t>>;
    const size_t size_first {first.size()};
    const size_t size_second {second.size()};
    const size_t size_out {size_first + size_second};
    std::vector<FlowerImage> vec_out(size_out);
    // const MapTp& map_first {first.getMap()};
    // const MapTp& map_second {second.getMap()};
    MapTp map_out = {
        {FlTp::Daisy,     {}},
        {FlTp::Dandelion, {}},
        {FlTp::Rose,      {}},
        {FlTp::Sunflower, {}},
        {FlTp::Tulip,     {}},
        {FlTp::NoFlower,  {}}
    };
    // Concatenate vectors
    for (size_t i {0}; i < size_first; i++)
    {
        vec_out[i] = first.m_vec[i];
    }
    for (size_t i {0}; i < size_second; i++)
    {
        vec_out[i + size_first] = second.m_vec[i];
    }
    // Concatenate maps
    for (auto& [fl_type, indices_vec] : map_out)
    {
        const size_t size_first {first.m_map.at(fl_type).size()};
        const size_t size_second {second.m_map.at(fl_type).size()};
        const size_t size_out {size_first + size_second};
        indices_vec.resize(size_out);
        for (size_t i {0}; i < size_first; i++)
        {
            indices_vec[i] = first.m_map.at(fl_type)[i];
        }
        for (size_t i {0}; i < size_second; i++)
        {
            indices_vec[i + size_first] = second.m_map.at(fl_type)[i];
        }
    }
}

