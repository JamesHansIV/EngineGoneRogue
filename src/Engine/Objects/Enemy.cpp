#include "Enemy.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Player.h"

#include <math.h>

/*
DEVELOPING BASIC ENEMY BEHAVIOUR:

- define the enemy perception range
- check for player detection in the enemy perception range
- calculate the direction vector from enemy's position to the player's position
- apply behaviour like move towards the player, attack in perception,etc

- collision detection between the perception and player
- collision detection between the enemy and the player
- collision detection between the enemies (might need to add offset - but this might cause problems)
*/

Enemy::Enemy(Properties& props, int perceptionX, int perceptionY):
    Character(props), m_PerceptionWidth(perceptionX), m_PerceptionHeight(perceptionY) {
    m_Animation = new Animation();
    m_Animation->AddAnimation("Idle", {m_TextureID, m_TilePos, 2, 50, SDL_FLIP_NONE, true});
    m_Animation->AddAnimation("Dead", {"player_dead", {0, 0, 18, 18}, 6, 50});
    m_Animation->SelectAnimation("Idle");
    SetHealth(new Health(100));
    m_MarkedForDeletion = false;
}

void Enemy::Draw(){
    if (m_TextureID == "boss") {
        m_Animation->Draw({m_DstRect.x, m_DstRect.y, m_DstRect.w + 15, m_DstRect.h});
    }
    else {
        m_Animation->Draw({m_DstRect.x, m_DstRect.y, m_DstRect.w, m_DstRect.h});
    }

    if (!m_State.HasState(CharacterState::Dead)) {
        m_Health->Draw(GetX(), GetY(), GetWidth());
    }
}

void Enemy::Update(float dt){
    if (GetState().HasState(CharacterState::Dead)) {
        m_Animation->Update();
        SDL_Log("enemy deadd");
        if (m_Animation->Stopped()) {
            SDL_Log("animation ended");
            GetState().SetState(CharacterState::ToBeDestroyed);
        }
        return;
    }

    MoveTowardsTarget(dt);
    m_RigidBody->Update(dt);

    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());

    if(m_Health->GetHP() <= 0)
    {
        GetState().SetState(CharacterState::Dead);
        m_Animation->SelectAnimation("Dead");
        ColliderHandler::GetInstance()->RemoveCollider(this);
        m_CollisionBox.clear();
    }
    
}

void Enemy::MoveTowardsTarget(float dt) {
    int rectLeft = GetX() - m_PerceptionWidth;
    int rectRight = GetX() + GetWidth() + m_PerceptionWidth;
    int rectTop = GetY() - m_PerceptionHeight;
    int rectBottom = GetY() + GetHeight() + m_PerceptionHeight;

    m_Perception = {rectLeft, rectTop, rectRight - rectLeft, rectBottom - rectTop };

    SDL_Rect target = m_Target->GetCollisionBox().GetRect();

    if(ColliderHandler::GetInstance()->CheckCollision(m_Perception, target))
    {
        float directionX = m_Target->GetMidPointX() - GetMidPointX();
        float directionY = m_Target->GetMidPointY() - GetMidPointY();

        float directionLength = sqrt(directionX * directionX + directionY * directionY);
        if (directionLength != 0) {
            directionX /= directionLength;
            directionY /= directionLength;
        }

        m_RigidBody->SetVelocity(Vector2D(directionX, directionY));
    }
}

void Enemy::OnCollide(Collider* collidee) {
    if (this == collidee) return;
    
    switch(collidee->GetObjectType()) {
        case ObjectType::Player:
            UnCollide(collidee);
            break;
        case ObjectType::Enemy:
            UnCollide(collidee);
            break;
        case ObjectType::MeleeWeapon:
            UnCollide(collidee);
            break;
        case ObjectType::Projectile:
            break;
        case ObjectType::Collider:
            UnCollide(collidee);
            break;
        default:
            SDL_LogError(0, "Invalid object type");
            assert(false);
            break;
    }
}

void Enemy::Clean(){
    delete m_Animation;
}