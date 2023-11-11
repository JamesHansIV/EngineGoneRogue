#pragma once

#include "Engine/Events/Event.h"
#include "Engine/Application/Application.h"
#include <iostream>
#include <tinyxml2.h>

class Game : public Application{
    public:
        Game();

        void Update(float dt) override;
        void Render() override;

    private:
        std::vector<GameObject*> m_Objects;
};

