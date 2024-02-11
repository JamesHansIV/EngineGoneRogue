#pragma once

#include <vector>
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/IObject.h"
#include "Engine/Objects/Characters/Player.h"

enum class ChestType {
    None = 0,
    Wooden,
    Silver,
    Golden
};

class Chest : public Collider {
   public:
    Chest(Properties& props, ChestType ctype, std::vector<GameObject*>& chestItems, Player* player);

    void Draw() override { GameObject::Draw(); }

    void Clean() override {};

    ObjectType GetObjectType() override { return ObjectType::Chest; }

    ChestType GetChestType() { return m_ChestType; };
    
    void Update(float dt) override;

    std::vector<GameObject*>* GetChestItems()  { return & m_ChestItems; }

    void OnCollide(Collider* collidee) override;

    float CalculateDistanceToPlayer();
    
   private:
    std::vector<GameObject*> m_ChestItems;
    ChestType m_ChestType;
    std::string m_IdleTexture;
    std::string m_OpeningTexture;
    Player* m_PlayerRef;
    float m_DistanceToPlayer;
};
