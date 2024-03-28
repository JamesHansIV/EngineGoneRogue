#pragma once

#include "Enemy.h"

class Charger : public Enemy {
   public:
    explicit Charger(Properties& props, const EnemyStats& stats);
    Charger(Collider* rhs, EnemyStats stats);

    ~Charger() override;

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

    [[nodiscard]] unsigned int GetAttackInterval() const {
        return m_attack_interval;
    }

    [[nodiscard]] unsigned int GetAttackDuration() const {
        return m_attack_duration;
    }

   private:
    // TODO: Probably should be apart of EnemyStats
    unsigned int m_last_attack = 0;
    unsigned int m_attack_interval = 3000;
    unsigned int m_attack_duration = 1000;
};