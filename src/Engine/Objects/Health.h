#pragma once

class Health {
   public:
    explicit Health(int max_health)
        : m_total_health(max_health), m_hp(max_health) {}

    [[nodiscard]] int GetHP() const { return m_hp; }

    void SetDamage(int damage) { m_hp = m_hp - damage; }

    void IncreaseHealth(int amount);

    void SetMaxHealth(int max_health) { m_total_health = max_health; }

    void Draw(int objX, int objY, int objWidth) const;

   private:
    int m_hp;
    int m_total_health;
    int m_x_offset{5};
    int m_y_offset{7};
};
