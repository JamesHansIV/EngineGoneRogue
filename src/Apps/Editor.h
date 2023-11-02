#pragma once

#include "Events/Event.h"
#include "Application.h"

class Editor : public Application{
    public:
        Editor();
        ~Editor();

        virtual void Update(float dt);
        virtual void Render();
    private:
        Map* m_Map;
};

