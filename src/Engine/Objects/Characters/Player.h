#pragma once

#include <stack>
#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/State/PlayerState.h"
#include "functional"

struct PlayerStats {
    float Speed;
    int DodgeCD;
    float DodgeSpeed;
    void Init();
};

class Player : public Character {

   public:
    explicit Player(Properties& props);
    virtual ~Player() override;
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    void UpdateWeapon(float dt);

    void HandleEvent(Event* event);
    void OnKeyPressed(SDL_Event& event);
    void OnKeyReleased(SDL_Event& event);
    virtual void OnCollide(Collider* collidee) override;

    PlayerStats& GetStats() { return m_Stats; }

    virtual ObjectType GetObjectType() override { return ObjectType::Player; }

   private:
    std::vector<Weapon*> m_Weapons;
    PlayerStats m_Stats;
    Weapon* m_CurrentWeapon;
    // TODO: add this to player state when game state is added.
    int m_able_to_dash = 0;
    bool m_is_dashing = false;
    int m_multiplier = 1;
};
