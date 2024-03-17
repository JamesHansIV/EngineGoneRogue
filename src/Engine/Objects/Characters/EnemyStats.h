#pragma once

struct EnemyStats {
    int health;
    int damage;
    float speed;
    float range;
    int perceptionWidth;
    int perceptionHeight;
    int xpGiven;

    EnemyStats operator*(float multiplier) {
        return EnemyStats{health *= static_cast<int>(multiplier),
                          damage *= static_cast<int>(multiplier),
                          speed *= multiplier,
                          range *= multiplier,
                          perceptionWidth,
                          perceptionHeight,
                          xpGiven};
    }
};

struct RangedEnemyStats : EnemyStats {
    int fireInterval;
    float spread = 0;
    int spreadCount = 0;

    RangedEnemyStats operator*(double multiplier) {
        return RangedEnemyStats{
            health *= static_cast<int>(multiplier),
            damage *= static_cast<int>(multiplier),
            speed *= multiplier,
            range *= multiplier,
            perceptionWidth,
            perceptionHeight,
            xpGiven,
            fireInterval,
            spread,
            spreadCount,
        };
    }
};

struct MeleeEnemyStats : EnemyStats {
    int attackCooldown;
    int attackDamage;
    int attackRange;
};
