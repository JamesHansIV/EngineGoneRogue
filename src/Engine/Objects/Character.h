#pragma once
#include "GameObject.h"
#include "Engine/Events/EventListener.h"
#include "Engine/Objects/Collider.h"
#include <string>

class Character: public Collider {

    public:
        explicit Character(Properties& props): Collider(props) {}

        void Draw() override =0;
        void Clean() override =0;
        virtual void Update(float dt) override=0;

    protected:
        std::string m_Name;
};
