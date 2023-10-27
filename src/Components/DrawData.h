#pragma once

#include "Component.h"
#include <string>

struct DrawData : public Component {
    public:
        DrawData(int objectID, std::string textureID) : Component(objectID), TextureID(textureID) {}

        virtual ComponentType GetComponentType() const override { return ComponentType::DrawData; }
        std::string TextureID;
};