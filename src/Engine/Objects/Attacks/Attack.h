#include "AttackPattern.h"
#include "Engine/Objects/Projectiles/Projectile.h"
#include "Engine/Objects/Projectiles/ProjectileManager.h"

class Attack {
   public:
    virtual void Perform() = 0;
};

struct RangedAttackInfo {
    float X;
    float Y;
    float TargetX;
    float TargetY;
    Properties ProjProps;
    float ProjSpeed = 3;
    int ProjCount = 1;
    float Spread = 0;
    bool IsPlayerOwned = false;
    float Range = 0;
};

class RangedAttack : public Attack {
   public:
    RangedAttack(std::vector<Projectile*> (*createBullets)(RangedAttackInfo),
                 int interval, AttackPattern* attackPattern = nullptr)
        : m_CreateBullets(createBullets),
          m_Interval(interval),
          m_AttackPattern(attackPattern) {}

    void Update(float dt);
    void Draw();

    void Reset();

    AttackPattern* GetAttackPattern() { return m_AttackPattern; }

    void SetAttackPattern(AttackPattern* burst) { m_AttackPattern = burst; }

    virtual void Perform() override;

    virtual void Shoot(RangedAttackInfo info);

   private:
    int m_Interval;
    std::vector<Projectile*> (*m_CreateBullets)(RangedAttackInfo);
    ProjectileManager m_ProjectileManager;
    AttackPattern* m_AttackPattern;
    int m_LastAttack = 0;
};
