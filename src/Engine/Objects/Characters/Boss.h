#pragma once

#include "Enemy.h"

class Boss : public Enemy {
   public:
    explicit Boss(Properties& props, const EnemyStats& stats);
    Boss(Collider* rhs, EnemyStats stats);

    ~Boss() override;

    void Init();

    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    GameObject* Copy() override;

    EnemyState* GetAttackState() override;

    void OnCollide(Collider* collidee) override;

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

    void SetLastAttack(unsigned int last_attack) {
        m_last_attack = last_attack;
    }

    [[nodiscard]] unsigned int GetLastAttack() const { return m_last_attack; }

    [[nodiscard]] unsigned int GetAttackDuration() const {
        return m_attack_duration;
    }

    [[nodiscard]] unsigned int GetAttackInterval() const {
        return m_attack_interval;
    }

    [[nodiscard]] bool IsAttacking() const { return m_attacking; }

    void SetAttacking(bool attacking) { m_attacking = attacking; }

   private:
    // TODO: Probably should be apart of EnemyStats
    unsigned int m_last_attack = 0;
    bool m_attacking = false;
    unsigned int m_attack_duration = 3000;
    unsigned int m_attack_interval = 3000;
};
