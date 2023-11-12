#pragma once
#include "GameObject.h"
#include "Engine/Events/EventListener.h"
#include <string>

class Character: public GameObject {

    public:
        explicit Character(Properties& props): GameObject(props) {}

        void Draw() override =0;
        void Clean() override =0;
        virtual void Update(float dt, const std::vector<GameObject*>& colliders)=0;

    protected:
        std::string m_Name;
};
