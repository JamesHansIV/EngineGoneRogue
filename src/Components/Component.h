#pragma once

enum class ComponentType {
    None = 0,
    Position, Dimensions, DrawData, Rotate, Physics, Animation, Transform, Lighting,
    ParticleEffect, Collision, UserControl, CameraFollow,
    Destroy, Audio, Player
};

struct Component {
    public:
        Component(int objectId) : ObjectID(objectId) {}
        virtual ~Component() {}
        virtual ComponentType GetComponentType() const = 0;
        //virtual void Update() = 0;
        int ObjectID;
};