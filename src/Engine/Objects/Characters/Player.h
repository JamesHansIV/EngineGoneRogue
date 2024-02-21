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

    [[nodiscard]] float GetSpeed() const { return m_Speed; };

    [[nodiscard]] int GetDodgeCd() const { return m_DodgeCD; };

    [[nodiscard]] float GetDodgeSpeed() const { return m_DodgeSpeed; };

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

    void AddExperience(int experience) { m_experience += experience; };

    void SetLevel(int level) { m_level = level; }

    [[nodiscard]] int GetLevel() const { return m_level; }

    void Update() {
        const int total_experience = m_experience;
        m_experience = total_experience % 100;
        int potential_levels = total_experience / 100;
        while (potential_levels > 0) {
            PushNewEvent(EventType::PlayerLevelUpEvent);

            potential_levels--;
            m_level++;
        }
    }

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

    [[nodiscard]] const PlayerStats& GetStats() const { return *m_stats; }

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
