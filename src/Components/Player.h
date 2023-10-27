#pragma once

#include "Component.h"
#include "../Events/Event.h"
#include <functional>

struct Player : public Component {
    public:
        Player(int objectID, std::function<void(int)> onUpdate) : Component(objectID), OnUpdate(onUpdate) {}

        virtual ComponentType GetComponentType() const override { return ComponentType::Player; }
        std::function<void(int)> OnUpdate;
};