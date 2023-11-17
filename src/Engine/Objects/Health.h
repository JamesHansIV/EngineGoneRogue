#pragma once

class Health {
    public:
        Health(int max_health) : m_TotalHealth(max_health), m_Health(max_health), m_XOffset(5), m_YOffset(5) {}
        int GetHealth(){ return m_Health;}
        void SetDamage(int damage){ m_Health = m_Health - damage;}
        void Draw(int objX, int objY, int objWidth);
    private:
        int m_Health;
        int m_TotalHealth;
        int m_XOffset;
        int m_YOffset;
};

