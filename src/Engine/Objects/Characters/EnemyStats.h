#pragma once

struct EnemyStats {
    int health;
    int damage;
    float speed;
    float range;
    int perceptionWidth;
    int perceptionHeight;
    int xpGiven;
};

struct RangedEnemyStats : EnemyStats {
    int fireInterval;
    float spread;
    int spreadCount;
};

struct MeleeEnemyStats : EnemyStats {
    int attackCooldown;
    int attackDamage;
    int attackRange;
};
