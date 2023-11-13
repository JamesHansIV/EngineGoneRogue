#pragma once

#include "GameObject.h"

class Weapon : public GameObject {
    public:
        Weapon(Properties& props, double angle)
            : GameObject(props), m_Angle(angle){};

        void Draw() override;
        void Clean() override;
        void Update(float dt) override;

    private:
        int m_Angle;
};