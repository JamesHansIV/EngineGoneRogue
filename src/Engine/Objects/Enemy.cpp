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
    m_Animation->SetProps(m_TextureID, m_TilePos, 2, 500);
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

    if (!m_MarkedForDeletion) {
        m_Health->Draw(GetX(), GetY(), GetWidth());
}
}

void Enemy::Update(float dt){
    MoveTowardsTarget(dt);
    m_RigidBody->Update(dt);

    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());

    if(m_Health->GetHP() <= 0)
    {
        m_Animation->SetProps("player_dead", {0, 0, 18, 18}, 6, 500);
        m_CollisionBox.clear();
        if (m_Animation->GetCurrentFrame() == 6-1) {
            m_MarkedForDeletion = true;
        }
    }
    m_Animation->Update();
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
            dynamic_cast<Player*>(collidee)->GetHealth()->SetDamage(1);
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