#pragma once

#include <stack>
#include "../../Events/Event.h"
#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/ExperienceBar.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/State/PlayerState.h"
#include "Engine/utils/utils.h"
#include "functional"

class PlayerStats {
   public:
    explicit PlayerStats(float speed, int dodgeCD, float dodgeSpeed,
                         int rangedDamage, int meleeDamage, int piercing,
                         int armorPercentage, int HPRegenRate,
                         int lifeStealPercentage) {
        m_Speed = speed;
        m_DodgeCD = dodgeCD;
        m_DodgeSpeed = dodgeSpeed;
        m_RangedDamage = rangedDamage;
        m_MeleeDamage = meleeDamage;
        m_experience = 0;
        m_Piercing = piercing;
        m_ArmorPercentage = armorPercentage;
        m_HPRegenRate = HPRegenRate;
        m_LifeStealPercentage = lifeStealPercentage;
        m_level = 1;
    }

    float getSpeed() { return m_Speed; };

    int getDodgeCD() { return m_DodgeCD; };

    float getDodgeSpeed() { return m_DodgeSpeed; };

    int getRangedDamage() const { return m_RangedDamage; }

    int getMeleeDamage() const { return m_MeleeDamage; }

    int getExperience() const { return m_experience; }

    int getPiercing() const { return m_Piercing; };

    int getArmorPercentage() const { return m_ArmorPercentage; }

    int getHPRegenRate() const { return m_HPRegenRate; }

    int getLifeStealPercentage() const { return m_LifeStealPercentage; }

    void setSpeed(float speed) { m_Speed = speed; };

    void setDodgeCD(int dodgeCD) { m_DodgeCD = dodgeCD; };

    void setDodgeSpeed(float dodgeSpeed) { m_DodgeSpeed = dodgeSpeed; };

    void setMeleeDamage(int meleeDamage) { m_MeleeDamage = meleeDamage; }

    void setRangedDamage(int rangedDamage) { m_RangedDamage = rangedDamage; }

    void setExperience(int experience) { m_experience = experience; }

    void setPiercing(int piercing) { m_Piercing = piercing; }

    void setArmorPercentage(int armorPercentage) {
        m_ArmorPercentage = armorPercentage;
    }

    void setHPRegenRate(int regenRate) { m_HPRegenRate = regenRate; }

    void setLifeStealPercentage(int lifeSteal) {
        m_LifeStealPercentage = lifeSteal;
    }

    void AddExperience(int experience) {
        const int total_experience = m_experience + experience;
        m_experience = total_experience % 100;
        int potential_levels = total_experience / 100;
        SDL_Log("Experience: %d", m_experience);
        SDL_Log("added experience: %d", experience);
        SDL_Log("Potential levels: %d", potential_levels);
        while (potential_levels > 0) {
            PushNewEvent(EventType::PlayerLevelUpEvent);

            potential_levels--;
            m_level++;
        }
    };

    ~PlayerStats();

   protected:
    float m_Speed;
    int m_DodgeCD;
    float m_DodgeSpeed;
    int m_RangedDamage;
    int m_MeleeDamage;
    int m_experience;
    int m_Piercing;
    int m_ArmorPercentage;
    int m_HPRegenRate;
    int m_level;
    int m_LifeStealPercentage;
};

class Player : public Character {

   public:
    explicit Player(Properties& props);
    ~Player() override;
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    void UpdateWeapon(float dt);

    void HandleEvent(Event* event);
    void OnKeyPressed(SDL_Event& event);
    void OnKeyReleased(SDL_Event& event);
    void OnCollide(Collider* collidee) override;

    PlayerStats& GetStats() { return *m_stats; }

    std::vector<Weapon*>& GetPlayerWeapons() { return m_Weapons; };

    Weapon* GetCurrentWeapon() { return m_CurrentWeapon; };

    ObjectType GetObjectType() override { return ObjectType::Player; }

   private:
    std::vector<Weapon*> m_Weapons;
    PlayerStats* m_stats;
    Weapon* m_CurrentWeapon;
    ExperienceBar* m_ExperienceBar;
    // TODO: add this to player state when game state is added.
    int m_able_to_dash = 0;
    bool m_is_dashing = false;
    int m_multiplier = 1;
};
