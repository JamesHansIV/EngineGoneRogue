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
        m_speed = movementInfo.speed;
        // Default Dodge CD is set in PlayerDodgeState
        m_dodge_cd = 0;
        m_dodge_speed = movementInfo.dodgeSpeed;
        m_dodge_distance = movementInfo.dodgeDistance;
        m_dodge_invincibility_time = movementInfo.dodgeInvincibilityTime;
        m_last_dodge_time = 0;
        m_ranged_damage = combatInfo.rangedDamage;
        m_melee_damage = combatInfo.meleeDamage;
        m_experience = 0;
        m_kill_count = 0;
        m_piercing = combatInfo.piercing;
        m_armor_percentage = combatInfo.armorPercentage;
        m_hp_regen_rate = healthInfo.HPRegenRate;
        m_life_steal_percentage = healthInfo.lifeStealPercentage;
        m_level = 1;
    }

    [[nodiscard]] float GetSpeed() const { return m_speed; };

    [[nodiscard]] int GetDodgeCd() const { return m_dodge_cd; };

    [[nodiscard]] float GetDodgeSpeed() const { return m_dodge_speed; };

    [[nodiscard]] float GetDodgeDistance() const { return m_dodge_distance; };

    [[nodiscard]] int GetDodgeInvincibility() const {
        return static_cast<int>(timer.GetTicks() - m_last_dodge_time <=
                                m_dodge_invincibility_time);
    }

    [[nodiscard]] int GetLastDodgeTime() const { return m_last_dodge_time; }

    [[nodiscard]] int GetRangedDamage() const { return m_ranged_damage; }

    [[nodiscard]] int GetMeleeDamage() const { return m_melee_damage; }

    [[nodiscard]] int GetExperience() const { return m_experience; }

    [[nodiscard]] int GetPiercing() const { return m_piercing; };

    [[nodiscard]] int GetArmorPercentage() const { return m_armor_percentage; }

    [[nodiscard]] int GetHPRegenRate() const { return m_hp_regen_rate; }

    [[nodiscard]] int GetLifeStealPercentage() const {
        return m_life_steal_percentage;
    }

    void SetSpeed(float speed) { m_speed = speed; };

    void SetDodgeCd(int dodgeCD) { m_dodge_cd = dodgeCD; };

    void SetDodgeSpeed(float dodgeSpeed) { m_dodge_speed = dodgeSpeed; };

    void SetDodgeDistance(float dodgeDistance) {
        m_dodge_distance = dodgeDistance;
    };

    void SetLastDodgeTime(int time) { m_last_dodge_time = time; }

    void SetMeleeDamage(int meleeDamage) { m_melee_damage = meleeDamage; }

    void SetRangedDamage(int rangedDamage) { m_ranged_damage = rangedDamage; }

    void SetExperience(int experience) { m_experience = experience; }

    void SetPiercing(int piercing) { m_piercing = piercing; }

    void SetArmorPercentage(int armorPercentage) {
        m_armor_percentage = armorPercentage;
    }

    void SetHpRegenRate(int regenRate) { m_hp_regen_rate = regenRate; }

    void SetLifeStealPercentage(int lifeSteal) {
        m_life_steal_percentage = lifeSteal;
    }

    void AddExperience(int experience);

    void IncrementKillCount() { m_kill_count++; }

    [[nodiscard]] int GetKillCount() const { return m_kill_count; }

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
    float m_speed;
    int m_dodge_cd;
    float m_dodge_speed;
    float m_dodge_distance;
    int m_dodge_invincibility_time;
    int m_last_dodge_time;
    int m_ranged_damage;
    int m_melee_damage;
    int m_experience;
    int m_piercing;
    int m_armor_percentage;
    int m_hp_regen_rate;
    int m_level;
    int m_life_steal_percentage;
    int m_kill_count = 0;
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
        return m_weapons;
    };

    [[nodiscard]] Weapon* GetCurrentWeapon() const { return m_current_weapon; };

    ObjectType GetObjectType() override { return ObjectType::Player; }

    void AddItem(Item* item);

    [[nodiscard]] const std::unordered_map<std::string, Item*>& GetPlayerItems()
        const {
        return m_items;
    };

   private:
    std::vector<Weapon*> m_weapons;
    std::unordered_map<std::string, Item*> m_items;
    PlayerStats* m_stats;
    Weapon* m_current_weapon;
    // TODO: add this to player state when game state is added.
    int m_able_to_dash = 0;
    bool m_is_dashing = false;
    int m_multiplier = 1;
    int m_last_health_regen = 0;
    std::vector<Properties*> m_projectile_props;
};
