// Author: Luca Pellegrini
#include <flower_type.hpp>

#include <string>

const std::string flowerTypeToString(const FlowerType& type, bool lowercase)
{
    // Make sure that the strings here match those listed in `class_names`!!!
    using FT = FlowerType;
    std::string str;
    switch (type)
    {
    case FT::Daisy:
        str = lowercase ? "daisy" : "Daisy";
        break;
    case FT::Dandelion:
        str = lowercase ? "dandelion" : "Dandelion";
        break;
    case FT::Rose:
        str = lowercase ? "rose" : "Rose";
        break;
    case FT::Sunflower:
        str = lowercase ? "sunflower" : "Sunflower";
        break;
    case FT::Tulip:
        str = lowercase ? "tulip" : "Tulip";
        break;
    case FT::NoFlower:
        str = lowercase ? "no flower" : "NoFlower";
        break;
    }
    return str;
}
