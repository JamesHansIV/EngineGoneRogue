#include "Player.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"

#include "Projectile.h"

std::vector<Weapon*> weapons;

Player::Player(Properties& props): Character(props){
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, m_TilePos, 2, 500);
    // m_Collider->SetCorrection(-45, -20, 60, 80 )
    m_Health = new Health(100);

    Properties propsG("gun", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    Weapon* w1 = new Weapon(propsG, PROJECTILE);
    weapons.push_back(w1);

    Properties propsM("melee", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    Weapon* w2 = new Weapon(propsM, MELEE);
    weapons.push_back(w2);
    m_CurrentWeapon = PROJECTILE;
    m_MarkedForDeletion = false;
}


void Player::Draw(){
    m_Animation->Draw({m_Transform->GetX(), m_Transform->GetY(), m_DstRect.w, m_DstRect.h});
    SDL_Log("player health: %d", m_Health->GetHealth());
    m_Health->Draw(GetX(), GetY(), GetWidth());
    for(auto *weapon: weapons)
    {
        if(weapon->GetType() == m_CurrentWeapon)
        {
            weapon->Draw();
        }
    }
}

void Player::Update(float dt){
    if(m_Health->GetHealth() <= 0)
    {
        m_Animation->SetProps("player_dead", {0, 0, 18, 18}, 6, 500);
        if (m_Animation->GetCurrentFrame() == 6-1) {
            m_MarkedForDeletion = true;
        }
    }
    m_Animation->Update();
    m_RigidBody.Update(dt);
    float multiplier = 1;
    m_RigidBody.UnSetForce();
    if (InputChecker::IsKeyPressed(SDLK_q) || m_is_dashing) {
        m_is_dashing = true;
        if (m_able_to_dash % 50 == 0 || m_able_to_dash > 50) {
          multiplier = 2;
        }
        if (m_able_to_dash == 75) {
          m_able_to_dash = 0;
          m_is_dashing = false;
        }
        m_able_to_dash++;
    }
    if (InputChecker::IsKeyPressed(SDLK_w)) {
        m_RigidBody.ApplyForceY(-13 * multiplier);
    }
    if (InputChecker::IsKeyPressed(SDLK_s)) {
        m_RigidBody.ApplyForceY(13* multiplier);
    }
    if (InputChecker::IsKeyPressed(SDLK_a)) {
        m_RigidBody.ApplyForceX(-13 * multiplier);
        // TODO: Add run animation
        //m_Animation->SetProps("player_run", 1, 8, 100, SDL_FLIP_HORIZONTAL);
        SetFlip(SDL_FLIP_HORIZONTAL);
    }
    if (InputChecker::IsKeyPressed(SDLK_d)) {
        m_RigidBody.ApplyForceX(13 * multiplier);
        //m_Animation->SetProps("player_run", 1, 8, 100);
        SetFlip(SDL_FLIP_NONE);
    }
    m_Transform->Translate(m_RigidBody.Position());
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    m_Animation->Update();
    CanMoveThrough();


    if (InputChecker::GetMouseWheelDirection() != 0) {
        if (m_CurrentWeapon == PROJECTILE) {
            m_CurrentWeapon = MELEE;
        } else {
            m_CurrentWeapon = PROJECTILE;
        }
        // Reset the mouse wheel direction to avoid toggling multiple times
        InputChecker::SetMouseWheelDirection(0);
    }

    int gunXX = GetMidPointX() - Renderer::GetInstance()->GetCameraX();
    int gunYY = GetMidPointY() - Renderer::GetInstance()->GetCameraY();

    float const delta_x = InputChecker::GetMouseX() - gunXX;
    float const delta_y = InputChecker::GetMouseY() - gunYY;
    float angle = atan2(delta_y, delta_x) * (180.0 / M_PI);
    if (angle < 0) angle += 360.0F;

    for(auto *weapon: weapons){
        if(weapon->GetType() == m_CurrentWeapon)
        {
            int gunX = GetX() + GetWidth() / 2;
            int gunY = GetY() + GetHeight() / 3;
            weapon->SetX(gunX);
            weapon->SetY(gunY);
            weapon->UpdateColliders(m_Colliders);
            weapon->SetRotation(angle);
            weapon->SetFlip(m_Flip);
            weapon->Update(dt);
        }
    }
}

void Player::CanMoveThrough()
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
        if (ColliderHandler::GetInstance()->CheckCollision(m_CollisionBox.GetRect(), collider->GetCollisionBox().GetRect()))
        {
            m_Transform->TranslateX(-m_RigidBody.Velocity().X/2);
            m_Transform->TranslateY(-m_RigidBody.Velocity().Y/2);
            m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
        }
    }
}

void Player::OnEvent(Event& event) {
    SDL_Event const e = event.GetEvent();
    if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == SDLK_a) {
            // dont think this is needed anymore, character does not face right or left
            //m_Animation->SetProps("player", 1, 6, 100, SDL_FLIP_HORIZONTAL);
            SetFlip(SDL_FLIP_HORIZONTAL);
        }
        if (e.key.keysym.sym == SDLK_d) {
            //m_Animation->SetProps("player", 1, 6, 100);
            SetFlip(SDL_FLIP_NONE);
        }
    }
}

void Player::Clean(){
    Renderer::GetInstance()->Clean();
}

