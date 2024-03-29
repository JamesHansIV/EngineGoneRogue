#pragma once

#include "Enemy.h"

class Slime : public Enemy {
   public:
    explicit Slime(Properties& props, const EnemyStats& stats,
                   bool split = false);
    Slime(Collider* rhs, EnemyStats stats, bool split = false);

    ~Slime() override;

    void Init();

    void Draw() override;
    void Clean() override;
    void Update(float dt) override;

    GameObject* Copy() override;

    void OnCollide(Collider* collidee) override;

    [[nodiscard]] bool IsSplit() const { return m_split; }

    ObjectType GetObjectType() override { return ObjectType::Enemy; }

   private:
    bool m_split;
};
