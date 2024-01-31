#pragma once

#include "Engine/Animation/Animation.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Physics/RigidBody.h"
#include "Projectile.h"
#include "SDL2/SDL.h"
#include "functional"

#define PI4 (M_PI / 4)

class WaveBullet : public Projectile {
   public:
    WaveBullet(Properties& props, float speed, float angle,
               bool playerOwned = false, bool flipped = false);
    virtual void Draw() override;
    virtual void Clean() override;
    virtual void Update(float dt) override;

    virtual void OnCollide(Collider* collidee) override;

    virtual ObjectType GetObjectType() override {
        return ObjectType::Projectile;
    }

   private:
    Vector2D m_Origin;
    Vector2D m_TPos;
    float m_WaveMagnitude;
    float m_Scale;
    int m_Flipped;
};
