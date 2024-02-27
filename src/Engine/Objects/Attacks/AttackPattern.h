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
        : PrevBurstTime(0), _Count(count), Count(count), CD(cd) {}

    void ResetCount() { Count = _Count; }

    void Reset() override { ResetCount(); }

    void Update(float dt) override {
        if (Count <= 0) {
            int ticks = timer.GetTicks();
            if ((ticks - PrevBurstTime) > CD) {
                ResetCount();
            }
        }
    }

    bool Attack() override {
        if (Count > 0) {
            Count--;
            PrevBurstTime = Count == 0 ? timer.GetTicks() : PrevBurstTime;
            return true;
        }
        return false;
    }

   private:
    double PrevBurstTime;
    int _Count;
    int Count;
    double CD;
};
