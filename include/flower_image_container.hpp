// Author: Luca Pellegrini
#ifndef FLOWER_IMAGE_CONTAINER_HPP
#define FLOWER_IMAGE_CONTAINER_HPP

#include <map>

#include <flower_type.hpp>
#include <flower_image.hpp>

/**
 * @brief A container of FlowerImage objects
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
     *
     * Time complexity O(n), where n = number of images of given FlowerType stored in the container
     * @param flower_type one of FlowerType values
     * @return a const vector of FlowerImage objects with given flower type
     */
    const std::vector<FlowerImage> getImagesByFlowerType(const FlowerType& flower_type) const;

    /**
     * @brief Checks if the container has no elements
     * @return true if the container is empty, false otherwise
     */
    bool empty() const;

    /**
     * @brief Returns the number of elements in the container
     */
    size_t size() const;

    /**
     * @brief Returns a reference to the FlowerImage object at the given index
     */
    FlowerImage& at(const size_t i);
    const FlowerImage& at(const size_t i) const;

    /**
     * @brief Adds a FlowerImage object to the end of the container
     */
    void push_back(const FlowerImage& img);

private:
    std::vector<FlowerImage> m_vec;
    std::map<FlowerType, std::vector<size_t>> m_map_flower_type_indices;
};

#endif // FLOWER_IMAGE_CONTAINER_HPP
