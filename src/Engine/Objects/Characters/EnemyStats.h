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
    float spread = 0;
    int spreadCount = 0;
};

struct MeleeEnemyStats : EnemyStats {
    int attackCooldown;
    int attackDamage;
    int attackRange;
};
