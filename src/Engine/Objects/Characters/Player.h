#pragma once

#include <stack>
#include "../../Events/Event.h"
#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/State/PlayerState.h"
#include "functional"

class PlayerStats {
   public:
    explicit PlayerStats(float speed, int dodgeCD, float dodgeSpeed,
                         int rangedDamage, int meleeDamage) {
        m_Speed = speed;
        m_DodgeCD = dodgeCD;
        m_DodgeSpeed = dodgeSpeed;
        m_RangedDamage = rangedDamage;
        m_MeleeDamage = meleeDamage;
        m_experience = 0;
    }

    float getSpeed() { return m_Speed; };

    int getDodgeCD() { return m_DodgeCD; };

    float getDodgeSpeed() { return m_DodgeSpeed; };

    int getRangedDamage() const { return m_RangedDamage; }

    int getMeleeDamage() const { return m_MeleeDamage; }

    int getExperience() const { return m_experience; }

    void setSpeed(float speed) { m_Speed = speed; };

    void setDodgeCD(int dodgeCD) { m_DodgeCD = dodgeCD; };

    void setDodgeSpeed(float dodgeSpeed) { m_DodgeSpeed = dodgeSpeed; };

    void setMeleeDamage(int meleeDamage) { m_MeleeDamage = meleeDamage; }

    void setRangedDamage(int rangedDamage) { m_RangedDamage = rangedDamage; }

    void setExperience(int experience) { m_experience = experience; }

    void AddExperience(int experience) {
        m_experience = m_experience + experience;
    }

    ~PlayerStats();

   protected:
    float m_Speed;
    int m_DodgeCD;
    float m_DodgeSpeed;
    int m_RangedDamage;
    int m_MeleeDamage;
    int m_experience;
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

    PlayerStats& GetStats() { return *m_stats; }

    virtual ObjectType GetObjectType() override { return ObjectType::Player; }

   private:
    std::vector<Weapon*> m_Weapons;
    PlayerStats* m_stats;
    Weapon* m_CurrentWeapon;
    // TODO: add this to player state when game state is added.
    int m_able_to_dash = 0;
    bool m_is_dashing = false;
    int m_multiplier = 1;
};
