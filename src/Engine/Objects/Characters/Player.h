#pragma once

#include <stack>
#include <unordered_map>
#include "../../Events/Event.h"
#include "Character.h"
#include "Engine/Animation/Animation.h"
#include "Engine/Objects/ColliderHandler.h"
#include "Engine/Objects/Item.h"
#include "Engine/Objects/ItemInventory.h"
#include "Engine/Objects/Weapons/Weapon.h"
#include "Engine/Physics/RigidBody.h"
#include "Engine/State/PlayerState.h"
#include "Engine/Timer/Timer.h"
#include "Engine/utils/utils.h"
#include "functional"

struct MovementInfo {
    float speed;
    float dodgeSpeed;
    float dodgeDistance;
    int dodgeInvincibilityTime;
};

struct CombatInfo {
    int rangedDamage;
    int meleeDamage;
    int piercing;
    int armorPercentage;
};

struct HealthInfo {
    int HPRegenRate;
    int lifeStealPercentage;
};

class PlayerStats {
   public:
    explicit PlayerStats(MovementInfo movementInfo, CombatInfo combatInfo,
                         HealthInfo healthInfo) {
        m_Speed = movementInfo.speed;
        // Default Dodge CD is set in PlayerDodgeState
        m_DodgeCD = 0;
        m_DodgeSpeed = movementInfo.dodgeSpeed;
        m_DodgeDistance = movementInfo.dodgeDistance;
        m_DodgeInvincibilityTime = movementInfo.dodgeInvincibilityTime;
        m_LastDodgeTime = 0;
        m_RangedDamage = combatInfo.rangedDamage;
        m_MeleeDamage = combatInfo.meleeDamage;
        m_experience = 0;
        m_killCount = 0;
        m_Piercing = combatInfo.piercing;
        m_ArmorPercentage = combatInfo.armorPercentage;
        m_HPRegenRate = healthInfo.HPRegenRate;
        m_LifeStealPercentage = healthInfo.lifeStealPercentage;
        m_level = 1;
    }

    [[nodiscard]] float GetSpeed() const { return m_Speed; };

    [[nodiscard]] int GetDodgeCd() const { return m_DodgeCD; };

    [[nodiscard]] float GetDodgeSpeed() const { return m_DodgeSpeed; };

    [[nodiscard]] float GetDodgeDistance() const { return m_DodgeDistance; };

    [[nodiscard]] int GetDodgeInvincibility() const {
        return timer.GetTicks() - m_LastDodgeTime <= m_DodgeInvincibilityTime;
    }

    int GetLastDodgeTime() { return m_LastDodgeTime; }

    [[nodiscard]] int GetRangedDamage() const { return m_RangedDamage; }

    [[nodiscard]] int GetMeleeDamage() const { return m_MeleeDamage; }

    [[nodiscard]] int GetExperience() const { return m_experience; }

    [[nodiscard]] int GetPiercing() const { return m_Piercing; };

    [[nodiscard]] int GetArmorPercentage() const { return m_ArmorPercentage; }

    [[nodiscard]] int GetHPRegenRate() const { return m_HPRegenRate; }

    [[nodiscard]] int GetLifeStealPercentage() const {
        return m_LifeStealPercentage;
    }

    void SetSpeed(float speed) { m_Speed = speed; };

    void SetDodgeCd(int dodgeCD) { m_DodgeCD = dodgeCD; };

    void SetDodgeSpeed(float dodgeSpeed) { m_DodgeSpeed = dodgeSpeed; };

    void SetDodgeDistance(float dodgeDistance) {
        m_DodgeDistance = dodgeDistance;
    };

    void SetLastDodgeTime(int time) { m_LastDodgeTime = time; }

    void SetMeleeDamage(int meleeDamage) { m_MeleeDamage = meleeDamage; }

    void SetRangedDamage(int rangedDamage) { m_RangedDamage = rangedDamage; }

    void SetExperience(int experience) { m_experience = experience; }

    void SetPiercing(int piercing) { m_Piercing = piercing; }

    void SetArmorPercentage(int armorPercentage) {
        m_ArmorPercentage = armorPercentage;
    }

    void SetHpRegenRate(int regenRate) { m_HPRegenRate = regenRate; }

    void SetLifeStealPercentage(int lifeSteal) {
        m_LifeStealPercentage = lifeSteal;
    }

    void AddExperience(int experience);

    void IncrementKillCount() { m_killCount++; }

    [[nodiscard]] int GetKillCount() const { return m_killCount; }

    void SetLevel(int level) { m_level = level; }

    [[nodiscard]] int GetLevel() const { return m_level; }

    void Update() {
        if (m_experience >= 100) {
            PushNewEvent(EventType::PlayerLevelUpEvent);
            m_experience -= 100;
            m_level++;
        }
    }

   protected:
    float m_Speed;
    int m_DodgeCD;
    float m_DodgeSpeed;
    float m_DodgeDistance;
    int m_DodgeInvincibilityTime;
    int m_LastDodgeTime;
    int m_RangedDamage;
    int m_MeleeDamage;
    int m_experience;
    int m_Piercing;
    int m_ArmorPercentage;
    int m_HPRegenRate;
    int m_level;
    int m_LifeStealPercentage;
    int m_killCount = 0;
};

class Player : public Character {

   public:
    explicit Player(Properties& props);
    Player(Collider* rhs);
    ~Player() override;
    Player(const Player&) = delete;

    void Init();
    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    void UpdateWeapon(float dt);

    void HandleEvent(Event* event);
    void OnKeyPressed(SDL_Event& event);
    void OnKeyReleased(SDL_Event& event);
    void OnCollide(Collider* collidee) override;

    [[nodiscard]] PlayerStats& GetStats() const { return *m_stats; }

    PlayerStats& GetMutableStats() { return *m_stats; }

    [[nodiscard]] const std::vector<Weapon*>& GetPlayerWeapons() const {
        return m_Weapons;
    };

    [[nodiscard]] Weapon* GetCurrentWeapon() const { return m_CurrentWeapon; };

    ObjectType GetObjectType() override { return ObjectType::Player; }

    void AddItem(Item* item);

    [[nodiscard]] const std::unordered_map<std::string, Item*>& GetPlayerItems()
        const {
        return m_Items;
    };

   private:
    std::vector<Weapon*> m_Weapons;
    std::unordered_map<std::string, Item*> m_Items;
    PlayerStats* m_stats;
    Weapon* m_CurrentWeapon;
    // TODO: add this to player state when game state is added.
    int m_able_to_dash = 0;
    bool m_is_dashing = false;
    int m_multiplier = 1;
    int m_lastHealthRegen = 0;
};
