#pragma once

class Health {
   public:
    explicit Health(int max_health)
        : m_TotalHealth(max_health), m_HP(max_health) {}

    [[nodiscard]] int GetHP() const { return m_HP; }

    void SetDamage(int damage) { m_HP = m_HP - damage; }

    void IncreaseHealth(int amount);

    void Draw(int objX, int objY, int objWidth) const;

   private:
    int m_HP;
    int m_TotalHealth;
    int m_XOffset{5};
    int m_YOffset{7};
};
