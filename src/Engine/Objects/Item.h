#pragma once

#include "Engine/Objects/GameObject.h"

enum class ItemType {
    Item1,
    Item2,
    Item3,
    Item4,
    Item5,
    Item6,
    Item7,
    Item8,
    Item9,
    Item10,
    Item11,
    Item12
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

    [[nodiscard]] const std::string& GetName() const {
        return ItemTypeToName.at(m_ItemType);
    }

    const std::string& GetDescription() const {
        return ItemTypeToDescription.at(m_ItemType);
    }

    const ItemType GetType() const {return m_ItemType;};

    bool operator==(const Item& other) const {
        return (m_ItemType == other.m_ItemType);
    }

    int GetCount() const {return m_Count;};

    void AddCount() {m_Count = m_Count + 1;};

    private:
    ItemType m_ItemType;
    static const std::unordered_map<ItemType, std::string> ItemTypeToTexture;
    static const std::unordered_map<ItemType, std::string> ItemTypeToDescription;
    static const std::unordered_map<ItemType, std::string> ItemTypeToName;
    std::string m_Description;
    int m_Count = 0;
};