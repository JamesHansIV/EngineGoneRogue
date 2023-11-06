#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Application/Application.h"
#include <iostream>
#include <tinyxml2.h>

class Game : public Application{
    public:
        Game();

        void Update(float dt);
        void Render();

        void LoadProject();
        void LoadObject(tinyxml2::XMLElement* xmlObj);
    private:
        Map* m_Map;
        std::vector<GameObject*> m_Objects;
};

