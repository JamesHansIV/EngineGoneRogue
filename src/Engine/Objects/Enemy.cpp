#include "Enemy.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"

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
    m_Health = new Health(100);
    m_MarkedForDeletion = false;
}

void Enemy::Draw(){
    if (m_TextureID == "boss") {
        m_Animation->Draw({m_Transform->GetX(), m_Transform->GetY(), m_DstRect.w + 15, m_DstRect.h});
    }
    else {
        m_Animation->Draw({m_Transform->GetX(), m_Transform->GetY(), m_DstRect.w, m_DstRect.h});
    }

    if (!m_MarkedForDeletion) {
        m_Health->Draw(GetX(), GetY(), GetWidth());
}
}

void Enemy::Update(float dt){
    m_RigidBody.UnSetForce();
    int const rect_left = GetX() - m_PerceptionWidth;
    int const rect_right = GetX() + GetWidth() + m_PerceptionWidth;
    int const rect_top = GetY() - m_PerceptionHeight;
    int const rect_bottom = GetY() + GetHeight() + m_PerceptionHeight;

    m_Perception = {rect_left - Renderer::GetInstance()->GetCameraX(),rect_top - Renderer::GetInstance()->GetCameraY(),rect_right - rect_left,rect_bottom - rect_top };

    SDL_Rect player = m_Player->GetCollisionBox().GetRect();
    player.x = player.x - Renderer::GetInstance()->GetCameraX();
    player.y = player.y - Renderer::GetInstance()->GetCameraY();

    if(ColliderHandler::GetInstance()->CheckCollision(m_Perception, player) && m_Animation->GetTextureID() != "player_dead")
    {
        float direction_x = m_Player->GetMidPointX() - GetMidPointX();
        float direction_y = m_Player->GetMidPointY() - GetMidPointY();

        float const direction_length = sqrt(direction_x * direction_x + direction_y * direction_y);
        if (direction_length != 0) {
            direction_x /= direction_length;
            direction_y /= direction_length;
        }

        float const force_magnitude = 4.0F;
        m_RigidBody.ApplyForce(Vector2D(direction_x * force_magnitude, direction_y * force_magnitude));
        m_RigidBody.Update(dt);
        m_Transform->Translate(m_RigidBody.Position());
        m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
        if (ColliderHandler::GetInstance()->CheckCollision(m_CollisionBox.GetRect(), m_Player->GetCollisionBox().GetRect()))
        {
            m_Transform->TranslateX(-m_RigidBody.Velocity().X/2);
            m_Transform->TranslateY(-m_RigidBody.Velocity().Y/2);
            m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
            m_Player->GetHealthObj()->SetDamage(1);
        }
        CanMoveThrough();
    }

    if(m_Health->GetHealth() <= 0)
    {
        m_Animation->SetProps("player_dead", {0, 0, 18, 18}, 6, 500);
        if (m_Animation->GetCurrentFrame() == 6-1) {
            m_MarkedForDeletion = true;
        }
    }
    m_Animation->Update();
}

void Enemy::CanMoveThrough()
{
    if(*m_Transform->X < 0.0F ||
        *m_Transform->Y < 0.0F ||
        *m_Transform->X + this->GetWidth() > SCREEN_WIDTH ||
        *m_Transform->Y + this->GetHeight() > SCREEN_HEIGHT)
    {
        m_Transform->TranslateX(-m_RigidBody.Velocity().X/2);
        m_Transform->TranslateY(-m_RigidBody.Velocity().Y/2);
        m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    }
    for (auto *collider : m_Colliders)
    {
        if (collider == this) continue;
        if (ColliderHandler::GetInstance()->CheckCollision(m_CollisionBox.GetRect(), collider->GetCollisionBox().GetRect()))
        {
            m_Transform->TranslateX(-m_RigidBody.Velocity().X/2);
            m_Transform->TranslateY(-m_RigidBody.Velocity().Y/2);
            m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
        }
    }
}

void Enemy::OnCollide(Collider* collidee) {
    if (this == collidee) return;

    switch(collidee->GetObjectType()) {
        case ObjectType::kCollider:
            SDL_Log("%s object collided with collider", GetID().c_str());
            break;
        case ObjectType::kProjectile:
            SDL_Log("%s object collided with projectile", GetID().c_str());
            break;

        default:
            SDL_LogError(0, "Invalid object type");
            assert(false);
            break;
    }
}

void Enemy::OnEvent(Event& event) {
}

void Enemy::Clean(){
    delete m_Animation;
}

