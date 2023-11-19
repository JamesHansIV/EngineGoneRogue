#include "Player.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/RangedWeapon.h"
#include "Engine/Objects/MeleeWeapon.h"

#include "Projectile.h"

Player::Player(Properties& props): Character(props){
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, m_TilePos, 2, 500);
    // m_Collider->SetCorrection(-45, -20, 60, 80 )
    m_Health = new Health(100);

    Properties propsG("gun", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    Weapon* w1 = new RangedWeapon(propsG, true);
    m_Weapons.push_back(w1);

    Properties propsM("melee", {0, 0, 18, 16}, {0, 0, 18, 16}, 0.0);
    Weapon* w2 = new MeleeWeapon(propsM, true);
    m_Weapons.push_back(w2);

    //ColliderHandler::GetInstance()->AddCollider(w2);

    m_CurrentWeapon = w1;
    m_MarkedForDeletion = false;
}

Player::~Player() {
    delete m_Health;
    for (auto& weapon : m_Weapons) {
        delete weapon;
    }
}


void Player::Draw(){
    m_Animation->Draw({GetX(), GetY(), m_DstRect.w, m_DstRect.h});
    m_Health->Draw(GetX(), GetY(), GetWidth());
    m_CurrentWeapon->Draw();
}

void Player::Update(float dt){
    if(m_Health->GetHP() <= 0)
    {
        m_Animation->SetProps("player_dead", {0, 0, 18, 18}, 6, 500);
        if (m_Animation->GetCurrentFrame() == 6-1) {
            m_MarkedForDeletion = true;
        }
    }
    m_Animation->Update();
    m_RigidBody->Update(dt);

    SetX(m_RigidBody->Position().X);
    SetY(m_RigidBody->Position().Y);
    m_CollisionBox.Set(this->GetX(), this->GetY(), GetHeight(), GetWidth());
    m_Animation->Update();

    UpdateWeapon(dt);
}

void Player::UpdateWeapon(float dt) {
    if (InputChecker::GetMouseWheelDirection() != 0) {
        auto it = std::find(m_Weapons.begin(), m_Weapons.end(), m_CurrentWeapon);
        it++;
        if (it == m_Weapons.end()) {
            it = m_Weapons.begin();
        }
        m_CurrentWeapon = *it;

        // Reset the mouse wheel direction to avoid toggling multiple times
        InputChecker::SetMouseWheelDirection(0);
    }

    int weaponXX = GetMidPointX() - Renderer::GetInstance()->GetCameraX();
    int weaponYY = GetMidPointY() - Renderer::GetInstance()->GetCameraY();

    float const delta_x = InputChecker::GetMouseX() - weaponXX;
    float const delta_y = InputChecker::GetMouseY() - weaponYY;
    float angle = atan2(delta_y, delta_x) * (180.0 / M_PI);
    if (angle < 0) angle += 360.0F;

    int weaponX = GetX() + GetWidth() / 2;
    int weaponY = GetY() + GetHeight() / 3;
    m_CurrentWeapon->SetX(weaponX);
    m_CurrentWeapon->SetY(weaponY); 
    m_CurrentWeapon->SetRotation(angle);
    m_CurrentWeapon->SetFlip(m_Flip);
    m_CurrentWeapon->Update(dt);
}

void Player::OnCollide(Collider* collidee) {
    if (this == collidee) return;

    switch(collidee->GetObjectType()) {
        case ObjectType::kPlayer:
            SDL_Log("%s object collided with player", GetID().c_str());
            break;
        case ObjectType::kEnemy:
            UnCollide(collidee);
            break;
        case ObjectType::kMeleeWeapon:
            UnCollide(collidee);
            break;
        case ObjectType::kProjectile:
            SDL_Log("%s object collided with projectile", GetID().c_str());
            break;
        case ObjectType::kCollider:
            UnCollide(collidee);
            break;
        default:
            SDL_LogError(0, "Invalid object type");
            assert(false);
            break;
    }
}

void Player::OnKeyPressed(SDL_Event& event) {
    if (event.key.repeat != 0) return;
    if (event.key.keysym.sym == SDLK_w) {
        m_RigidBody->ApplyVelocity(Vector2D(0, -3));
    } else if (event.key.keysym.sym == SDLK_s) {
        m_RigidBody->ApplyVelocity(Vector2D(0, 3));
    } else if (event.key.keysym.sym == SDLK_a) {
        m_RigidBody->ApplyVelocity(Vector2D(-3, 0));
    } else if (event.key.keysym.sym == SDLK_d) {
        m_RigidBody->ApplyVelocity(Vector2D(3, 0));
    }
}

void Player::OnKeyReleased(SDL_Event& event) {
    //Need a better way to do this... this does not work if outside forces affect velocity

    if (event.key.keysym.sym == SDLK_w) {
        Vector2D adjust = Vector2D(0, 3);
        //adjust.Y = m_RigidBody->Velocity().Y != 0 ? adjust.Y : 0;
        m_RigidBody->ApplyVelocity(adjust);
    } else if (event.key.keysym.sym == SDLK_s) {
        Vector2D adjust = Vector2D(0, -3);
        //adjust.Y = m_RigidBody->Velocity().Y != 0 ? adjust.Y : 0;
        m_RigidBody->ApplyVelocity(Vector2D(adjust));
    } else if (event.key.keysym.sym == SDLK_a) {
        Vector2D adjust = Vector2D(3, 0);
        //adjust.X = m_RigidBody->Velocity().X != 0 ? adjust.X : 0;
        m_RigidBody->ApplyVelocity(Vector2D(adjust));
    } else if (event.key.keysym.sym == SDLK_d) {
        Vector2D adjust = Vector2D(-3, 0);
        //adjust.X = m_RigidBody->Velocity().X != 0 ? adjust.X : 0;
        m_RigidBody->ApplyVelocity(Vector2D(adjust));
    }
}

void Player::Clean(){

}

