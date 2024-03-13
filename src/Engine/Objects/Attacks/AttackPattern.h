#include "Engine/Timer/Timer.h"

class AttackPattern {
   public:
    virtual void Update(float dt) = 0;
    virtual bool Attack() = 0;
    virtual void Reset() = 0;
    virtual ~AttackPattern() = default;
    AttackPattern() = default;
    AttackPattern(const AttackPattern&) = default;
    AttackPattern& operator=(const AttackPattern&) = default;
};

class Burst : public AttackPattern {
   public:
    Burst(int count, float cd)
        : 
          m_fixed_count(count),
          m_cur_count(count),
          m_cd(cd) {}

    void ResetCount() { m_cur_count = m_fixed_count; }

    void Reset() override { ResetCount(); }

    void Update(float dt) override {
        if (m_cur_count <= 0) {
            int ticks = timer.GetTicks();
            if ((ticks - m_prev_burst_time) > m_cd) {
                ResetCount();
            }
        }
    }

    bool Attack() override {
        if (m_cur_count > 0) {
            m_cur_count--;
            m_prev_burst_time =
                m_cur_count == 0 ? timer.GetTicks() : m_prev_burst_time;
            return true;
        }
        return false;
    }

   private:
    double m_prev_burst_time{0};
    int m_fixed_count;
    int m_cur_count;
    double m_cd;
};
