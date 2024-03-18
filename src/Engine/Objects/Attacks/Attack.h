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
    AnimationInfo HitAnimation;
    float ProjSpeed = 400;
    int ProjCount = 1;
    float Spread = 0;
    bool IsPlayerOwned = false;
    float Range = 0;
};

class RangedAttack : public Attack {
   public:
    RangedAttack(std::vector<Projectile*> (*createBullets)(RangedAttackInfo),
                 int interval, AttackPattern* attackPattern = nullptr)
        : m_create_bullets(createBullets),
          m_interval(interval),
          m_attack_pattern(attackPattern) {}

    void Update(float dt);
    void Draw();

    void Reset();

    AttackPattern* GetAttackPattern() { return m_attack_pattern; }

    void SetAttackPattern(AttackPattern* burst) { m_attack_pattern = burst; }

    void Perform() override;

    virtual void Shoot(RangedAttackInfo info);
    ~RangedAttack();

   private:
    int m_interval;
    std::vector<Projectile*> (*m_create_bullets)(RangedAttackInfo);
    ProjectileManager m_projectile_manager;
    AttackPattern* m_attack_pattern;
    Uint32 m_last_attack = 0;
};
