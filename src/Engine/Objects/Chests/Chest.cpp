#include "Chest.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"
#include "SDL2/SDL_log.h"

constexpr float kCloseDistance = 45.0F;

Chest::Chest(Properties& props, ChestType ctype,
             std::vector<GameObject*>& chestItems, Player* player)
    : Collider(props) {
    m_ChestItems = chestItems;
    m_ChestType = ctype;
    m_Animation = new Animation();
    m_PlayerRef = player;

    if (m_ChestType == ChestType::Wooden) {
        m_IdleTexture = "wooden-chest-idle";
        m_OpeningTexture = "wooden-chest-opening";
    } else if (m_ChestType == ChestType::Silver) {
        m_IdleTexture = "silver-chest-idle";
        m_OpeningTexture = "silver-chest-opening";
    } else if (m_ChestType == ChestType::Golden) {
        m_IdleTexture = "golden-chest-idle";
        m_OpeningTexture = "golden-chest-opening";
    }

    m_Animation->AddAnimation(
        m_IdleTexture,
        {m_IdleTexture, {0, 0, 18, 16}, 1, 15, SDL_FLIP_NONE, true});

    m_Animation->AddAnimation(
        m_OpeningTexture,
        {m_OpeningTexture, {0, 0, 18, 16}, 5, 15, SDL_FLIP_NONE, true});

    m_Animation->SelectAnimation(m_IdleTexture);
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());
}

void Chest::Update(float /*dt*/) {
    m_DistanceToPlayer = CalculateDistanceToPlayer();
    m_Animation->Update();

    if (m_DistanceToPlayer <= kCloseDistance &&
        InputChecker::IsKeyPressed(SDLK_o)) {
        if (m_Animation->GetAnimationID() != m_OpeningTexture) {
            m_Animation->SelectAnimation(m_OpeningTexture);
        }
    }

    if (m_Animation->GetAnimationID() == m_OpeningTexture &&
        m_Animation->GetCurrentFrame() == 4) {
        MarkForDeletion();
        ColliderHandler::GetInstance()->RemoveCollider(this);
    }
}

float Chest::CalculateDistanceToPlayer() {
    float const dx = m_PlayerRef->GetX() - GetX();
    float const dy = m_PlayerRef->GetY() - GetY();
    return std::sqrt(dx * dx + dy * dy);
}

void Chest::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }
    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
        default:
            break;
    }
}
