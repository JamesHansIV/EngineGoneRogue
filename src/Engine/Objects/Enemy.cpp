#include "Enemy.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Physics/CollisionHandler.h"

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
    m_RigidBody = new RigidBody();
    m_Collider = new Collider();
    m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
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
    
    if (m_MarkedForDeletion == false)
        DrawEnemyHealth();
}

void Enemy::DrawEnemyHealth(){
    const int HEALTH_BAR_HEIGHT = 10;

    int healthBarWidth = static_cast<int>((GetWidth() + 15) * (m_Health->GetHealth() / 100.0));
    if (healthBarWidth <= 0) healthBarWidth = 0;
    int healthBarX = GetX();
    int healthBarY = GetY() - HEALTH_BAR_HEIGHT - 5;

    SDL_Rect healthBarRect;
    healthBarRect.x = healthBarX - Renderer::GetInstance()->GetCameraX() - 5;
    healthBarRect.y = healthBarY - Renderer::GetInstance()->GetCameraY();
    healthBarRect.w = healthBarWidth;
    healthBarRect.h = HEALTH_BAR_HEIGHT;

    SDL_SetRenderDrawColor(Renderer::GetInstance()->GetRenderer(), 0, 255, 0, 255);
    SDL_RenderFillRect(Renderer::GetInstance()->GetRenderer(), &healthBarRect);
}

void Enemy::Update(float dt){
    m_RigidBody->UnSetForce();
    int rectLeft = GetX() - m_PerceptionWidth;
    int rectRight = GetX() + GetWidth() + m_PerceptionWidth;
    int rectTop = GetY() - m_PerceptionHeight;
    int rectBottom = GetY() + GetHeight() + m_PerceptionHeight;

    m_Perception = {rectLeft - Renderer::GetInstance()->GetCameraX(),rectTop - Renderer::GetInstance()->GetCameraY(),rectRight - rectLeft,rectBottom - rectTop };

    SDL_Rect player = m_Player->GetCollider()->Get();
    player.x = player.x - Renderer::GetInstance()->GetCameraX();
    player.y = player.y - Renderer::GetInstance()->GetCameraY();

    if(CollisionHandler::GetInstance()->CheckCollision(m_Perception, player))
    {
        float directionX = m_Player->GetMidPointX() - GetMidPointX();
        float directionY = m_Player->GetMidPointY() - GetMidPointY();

        float directionLength = sqrt(directionX * directionX + directionY * directionY);
        if (directionLength != 0) {
            directionX /= directionLength;
            directionY /= directionLength;
        }

        float forceMagnitude = 8.0f;
        m_RigidBody->ApplyForce(Vector2D(directionX * forceMagnitude, directionY * forceMagnitude));
        m_RigidBody->Update(dt);
        m_Transform->Translate(m_RigidBody->Position());
        m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
        if (CollisionHandler::GetInstance()->CheckCollision(m_Collider->Get(), m_Player->GetCollider()->Get()))
        {
            m_Transform->TranslateX(-m_RigidBody->Velocity().X/2);
            m_Transform->TranslateY(-m_RigidBody->Velocity().Y/2);
            m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth()); 
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
        m_Transform->TranslateX(-m_RigidBody->Velocity().X/2);
        m_Transform->TranslateY(-m_RigidBody->Velocity().Y/2);
        m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());    
    }
    for (auto *collider : m_Colliders)
    {
        if (collider->GetCollider() == this->GetCollider()) continue;
        if (CollisionHandler::GetInstance()->CheckCollision(m_Collider->Get(), collider->GetCollider()->Get()))
        {
            m_Transform->TranslateX(-m_RigidBody->Velocity().X/2);
            m_Transform->TranslateY(-m_RigidBody->Velocity().Y/2);
            m_Collider->Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());            
        }
    }
}


void Enemy::OnEvent(Event& event) {
}

void Enemy::Clean(){
    delete m_Animation;
    delete m_RigidBody;
}

