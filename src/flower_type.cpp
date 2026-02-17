// Author: Luca Pellegrini
#include <flower_type.hpp>

#include <map>

const std::string flowerTypeToString(const FlowerType& type, bool lowercase)
{
    // Make sure that the strings here match those listed in `class_names`!!!
    // map element: {FlowerType, pair{uppercase string, lowercase string}
    static const std::map<FlowerType, std::pair<std::string, std::string>> map {
        {FlowerType::Daisy,     {"Daisy",     "daisy"}},
        {FlowerType::Dandelion, {"Dandelion", "dandelion"}},
        {FlowerType::Rose,      {"Rose",      "rose"}},
        {FlowerType::Sunflower, {"Sunflower", "sunflower"}},
        {FlowerType::Tulip,     {"Tulip",     "tulip"}},
        {FlowerType::NoFlower,  {"NoFlower",  "no flower"}}
    };
    const std::pair<std::string, std::string> pair {map.at(type)};
    if (lowercase)
    {
        return pair.second;
    }
    else
    {
        return pair.first;
    }
}
