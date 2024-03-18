#pragma once

#include "Engine/Objects/GameObject.h"

enum class ItemType {
    PiercingShard,
    RegenElixir,
    LifestealStone,
    RangedBoost,
    MeleeBoost,
    ArmorEssence,
    SwiftBoots,
    DodgeCharm
};

class Player;

class Item : public GameObject {
   public:
    explicit Item(Properties& props, ItemType itemType) : GameObject(props), m_item_type(itemType) {
        SetTextureID(GetTexture());
    };
    
    void Draw() override { GameObject::Draw(); }
    
    void Apply(Player* player);
    
    ~Item() override = default;

    [[nodiscard]] const std::string& GetTexture() const {
        return kItemTypeToTexture.at(m_item_type);
    }

    [[nodiscard]] const std::string& GetName() const {
        return kItemTypeToName.at(m_item_type);
    }

    [[nodiscard]] const std::string& GetDescription() const {
        return kItemTypeToDescription.at(m_item_type);
    }

    [[nodiscard]] ItemType GetType() const {return m_item_type;};

    bool operator==(const Item& other) const {
        return (m_item_type == other.m_item_type);
    }

    [[nodiscard]] int GetCount() const {return m_count;};

    void AddCount() {m_count = m_count + 1;};

    private:
    ItemType m_item_type;
    static const std::unordered_map<ItemType, std::string> kItemTypeToTexture;
    static const std::unordered_map<ItemType, std::string> kItemTypeToDescription;
    static const std::unordered_map<ItemType, std::string> kItemTypeToName;
    std::string m_description;
    int m_count = 0;
};