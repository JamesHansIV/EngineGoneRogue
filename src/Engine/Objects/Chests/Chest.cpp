#include "Chest.h"
#include "Engine/Animation/Animation.h"

Chest::Chest(Properties& props, ChestType ctype, std::vector<GameObject*>& chestItems ) : Collider(props) 
{
    m_ChestItems = chestItems;
    m_ChestType = ctype;
    m_Animation = new Animation();

    std::string idle_texture;
    std::string opening_texture;

    if(m_ChestType == ChestType::Wooden) {
        idle_texture = "wooden_chest_idle";
        opening_texture = "wooden_chest_opening";
    } else if (m_ChestType == ChestType::Silver) {
        idle_texture = "silver_chest_idle";
        opening_texture = "silver_chest_opening";
    } else if (m_ChestType == ChestType::Golden) {
        idle_texture = "golden_chest_idle";
        opening_texture = "golden_chest_opening";
    }

    m_Animation->AddAnimation(
        idle_texture,
        {idle_texture, {0, 0, 16, 16},1, 15, SDL_FLIP_NONE, true});

    m_Animation->AddAnimation(
        opening_texture,
        {opening_texture, {0, 0, 16, 16},7, 50, SDL_FLIP_NONE, true});

    SDL_Log("idle_texuture %s", idle_texture.c_str());
    m_Animation->SelectAnimation(idle_texture);
}

void Chest::Update(float dt) {
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());
    m_Animation->Update();
}

