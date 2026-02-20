// Author: Luca Pellegrini
#ifndef FLOWER_TYPE_HPP
#define FLOWER_TYPE_HPP

#include <string>
#include <vector>

/**
 * @brief Types (a.k.a. classes) of flowers that are recognized by the flower_classifier
 */
enum class FlowerType {Daisy = 0,
                       Dandelion = 1, 
                       Rose = 2, 
                       Sunflower = 3, 
                       Tulip = 4, 
                       NoFlower = 5};

/**
 * @brief Number of classes recognized by the flower_classifier
 */
const size_t num_classes {6};

/**
 * @brief Vector containing the names of the classes recognized by the flower_classifier
 *
 * Classes are listed in the same order in which they are defined in the FlowerType enum.
 * Classes' names are the same as those returned by the flowerTypeToString function (with lowercase == false)
 */
const std::vector<std::string> class_names = {
    "Daisy", "Dandelion", "Rose", "Sunflower", "Tulip", "NoFlower"
};

/**
 * @brief flowerTypeToString Get the string representing a given flower type
 * @param type one of the values of FlowerType
 * @param lowercase if true, returns a lowercase version of the string
 * @return string describing the given type
 */
const std::string flowerTypeToString(const FlowerType& type, bool lowercase = false);

#endif // FLOWER_TYPE_HPP
