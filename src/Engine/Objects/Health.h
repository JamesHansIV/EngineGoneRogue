#pragma once

class Health {
    public:
        Health(int max_health) { m_Health = max_health; };
        int GetHealth(){ return m_Health;};
        void SetDamage(int damage){ m_Health = m_Health - damage;};
    private:
        int m_Health;
};

