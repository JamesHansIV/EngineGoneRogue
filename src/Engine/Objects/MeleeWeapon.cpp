#include "MeleeWeapon.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Objects/ColliderHandler.h"

#include "Engine/Objects/Character.h"


MeleeWeapon::MeleeWeapon(Properties& props, bool playerOwned) : Weapon(props, playerOwned) {}

void MeleeWeapon::Draw()
{
    GameObject::Draw();
}

void MeleeWeapon::Update(float dt)
{
    m_CollisionBox.Set(GetX(), GetY(), GetHeight(), GetWidth());

    if(GetFlip() == SDL_FLIP_HORIZONTAL)
    {   
        SetX(GetX() - 12);
        m_CollisionBox.Set(GetX()-12, GetY()-12, GetHeight(), GetWidth());
    }

    if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT))
    {
        float swingAngle = 0.0f;
        if(GetFlip() == SDL_FLIP_HORIZONTAL)
        {   
            swingAngle = -45.0f;
        }
        else
        {
            swingAngle = 45.0f;
        }
        SetRotation(swingAngle);
        InputChecker::SetMouseButtonPressed(SDL_BUTTON_LEFT, false);
    }
    else
    {
        SetRotation(0.0);
    }
}

void MeleeWeapon::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch(collidee->GetObjectType()) {
        case ObjectType::kPlayer:
            if (!GetPlayerOwned()) {
                dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::kEnemy:
            if (GetPlayerOwned()) {
                dynamic_cast<Character*>(collidee)->GetHealth()->SetDamage(10);
            }
            break;
        case ObjectType::kMeleeWeapon:
            break;
        case ObjectType::kProjectile:
            break;
        case ObjectType::kCollider:
            break;
        default:
            SDL_LogError(0, "Invalid object type");
            assert(false);
            break;
    }
}

void MeleeWeapon::Clean()
{

}