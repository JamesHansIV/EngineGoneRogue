#include "Item.h"

const std::unordered_map<ItemType, std::string> Item::ItemTypeToTexture = {
    {ItemType::Item1, "pistol"},
    {ItemType::Item2, "uzi"},
    {ItemType::Item3, "sniper"},
    {ItemType::Item4, "bow"}
};

const std::unordered_map<ItemType, std::string> Item::ItemTypeToDescription = {
    {ItemType::Item1, "Description of Item 1"},
    {ItemType::Item2, "Description of Item 2"},
    {ItemType::Item3, "Description of Item 3"},
    {ItemType::Item4, "Description of Item 4"}
};

void Item::Use() {
    
}