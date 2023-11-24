#pragma once

#include "Collider.h"
#include "Engine/Animation/Animation.h"

class Entrance: public Collider{
    public:
        explicit Entrance(Properties& props);
        virtual void Draw() override;
        virtual void Clean() override;
        virtual void Update(float dt) override;

        virtual void OnCollide(Collider* collidee) override;

        virtual ObjectType GetObjectType() override { return ObjectType::Entrance; }
    private:
        Animation* m_Open;
        Animation* m_Close;
};
