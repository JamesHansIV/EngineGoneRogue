#pragma once
#include "GameObject.h"
#include "Engine/Events/EventListener.h"
#include <string>

class Character: public GameObject {

    public:
        Character(Properties& props): GameObject(props) {}

        virtual void Draw()=0;
        virtual void Clean()=0;
        virtual void Update(float dt, const std::vector<GameObject*>& colliders)=0;

    protected:
        std::string m_Name;
};
