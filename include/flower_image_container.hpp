// Author: Luca Pellegrini
#ifndef FLOWER_IMAGE_CONTAINER_HPP
#define FLOWER_IMAGE_CONTAINER_HPP

#include <map>

#include <flower_type.hpp>
#include <flower_image.hpp>

/**
 * @brief The FlowerImageContainer class represents a set of FlowerImage objects
 */
class FlowerImageContainer
{
public:
    FlowerImageContainer();

    /**
     * @brief getImagesVector
     * @return a const reference to the underlying vector of FlowerImage objects
     */
    const std::vector<FlowerImage>& getImagesVector() const;

    /**
     * @brief getImagesByFlowerType
     * @param flower_type one of FlowerType values
     * @return a const vector of FlowerImage objects with given flower type
     */
    const std::vector<FlowerImage> getImagesByFlowerType(const FlowerType& flower_type) const;

    /**
     * @brief empty Checks if the container has no elements
     * @return true if the container is empty, false otherwise
     */
    bool empty() const;

    /**
     * @brief size Returns the number of elements in the container
     * @return
     */
    size_t size() const;

    void push_back(const FlowerImage& img);

private:
    std::vector<FlowerImage> m_vec;
    std::map<FlowerType, std::vector<size_t>> m_map_flower_type_indices;
};

#endif // FLOWER_IMAGE_CONTAINER_HPP
