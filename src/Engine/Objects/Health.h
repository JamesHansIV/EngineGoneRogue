#pragma once

class Health {
    public:
        Health(int max_health) : m_TotalHealth(max_health), m_HP(max_health), m_XOffset(5), m_YOffset(5) {}
        int GetHP(){ return m_HP;}
        void SetDamage(int damage){ m_HP = m_HP - damage;}
        void Draw(int objX, int objY, int objWidth) const;
    private:
        int m_HP;
        int m_TotalHealth;
        int m_XOffset;
        int m_YOffset;
};

