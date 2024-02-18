#pragma once

#include "Apps/Game.h"
#include "Engine/Objects/GameObject.h"

enum class ItemType {
    Item1,
    Item2,
    Item3,
    Item4
};

class Item : public GameObject {
   public:
    explicit Item(Properties& props, ItemType itemType) : GameObject(props), m_ItemType(itemType) {
        SetTextureID(GetTexture());
    };
    
    void Draw() override { GameObject::Draw(); }
    
    void Use() ;
    
    ~Item() override = default;

    [[nodiscard]] const std::string& GetTexture() const {
        return ItemTypeToTexture.at(m_ItemType);
    }

    const std::string& GetDescription() const {
        return ItemTypeToDescription.at(m_ItemType);
    }

    private:
    ItemType m_ItemType;
    static const std::unordered_map<ItemType, std::string> ItemTypeToTexture;
    static const std::unordered_map<ItemType, std::string> ItemTypeToDescription;
    std::string m_Description;
};