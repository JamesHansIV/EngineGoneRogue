
class AttackPattern {
   public:
    virtual void Update(float dt) = 0;
    virtual bool Attack() = 0;
    virtual void Reset() = 0;
};

class Burst : public AttackPattern {
   public:
    Burst(int count, float cd) : _Count(count), Count(count), _CD(cd), CD(cd) {}

    void ResetCount() { Count = _Count; }

    void ResetCD() { CD = _CD; }

    void Reset() override {
        ResetCount();
        ResetCD();
    }

    void Update(float dt) override {
        if (Count <= 0) {
            if (CD > 0) {
                CD--;
            } else {
                ResetCount();
                ResetCD();
            }
        }
    }

    bool Attack() override {
        if (Count > 0) {
            Count--;
            return true;
        }
        return false;
    }

   private:
    int _Count;
    float _CD;
    int Count;
    float CD;
};
