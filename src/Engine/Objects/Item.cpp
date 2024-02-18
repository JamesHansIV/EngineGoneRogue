#include "Item.h"

const std::unordered_map<ItemType, std::string> Item::ItemTypeToTexture = {
    {ItemType::Item1, "healthpotion"},
    {ItemType::Item2, "wooden-chest-idle"},
    {ItemType::Item3, "sniper"},
    {ItemType::Item4, "healthpotion"},
    {ItemType::Item5, "wooden-chest-idle"},
    {ItemType::Item6, "healthpotion"},
    {ItemType::Item7, "sniper"},
    {ItemType::Item8, "healthpotion"},
    {ItemType::Item9, "wooden-chest-idle"},
    {ItemType::Item10, "wooden-chest-idle"},
    {ItemType::Item11, "healthpotion"},
    {ItemType::Item12, "healthpotion"}
};

const std::unordered_map<ItemType, std::string> Item::ItemTypeToDescription = {
    {ItemType::Item1, "Description of Item 1"},
    {ItemType::Item2, "Description of Item 2"},
    {ItemType::Item3, "Description of Item 3"},
    {ItemType::Item4, "Description of Item 4"},
    {ItemType::Item5, "Description of Item 5"},
    {ItemType::Item6, "Description of Item 6"},
    {ItemType::Item7, "Description of Item 7"},
    {ItemType::Item8, "Description of Item 8"},
    {ItemType::Item9, "Description of Item 9"},
    {ItemType::Item10, "Description of Item 10"},
    {ItemType::Item11, "Description of Item 11"},
    {ItemType::Item12, "Description of Item 12"}
};

const std::unordered_map<ItemType, std::string> Item::ItemTypeToName = {
    {ItemType::Item1, "Item 1"},
    {ItemType::Item2, "Item 2"},
    {ItemType::Item3, "Item 3"},
    {ItemType::Item4, "Item 4"},
    {ItemType::Item5, "Item 5"},
    {ItemType::Item6, "Item 6"},
    {ItemType::Item7, "Item 7"},
    {ItemType::Item8, "Item 8"},
    {ItemType::Item9, "Item 9"},
    {ItemType::Item10, "Item 10"},
    {ItemType::Item11, "Item 11"},
    {ItemType::Item12, "Item 12"}
};

void Item::Use() {
    
}