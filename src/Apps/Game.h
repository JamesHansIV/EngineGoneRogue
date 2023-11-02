#pragma once

#include "Events/Event.h"
#include "Application.h"

class Game : public Application{
    public:
        Game();

        void Update(float dt);
        void Render();

    private:
        Map* m_Map;
};

