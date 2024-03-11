#pragma once
#include "Vector2D.h"

#define UNI_MASS 1.0f
#define GRAVITY 9.8f

class RigidBody {
   public:
    explicit RigidBody(float x, float y, float mass = UNI_MASS) {
        m_mass = mass;
        m_gravity = GRAVITY;
        m_position = Vector2D(x, y);
    }

    inline void SetMass(float mass) { m_mass = mass; }

    inline void SetGravity(float gravity) { m_gravity = gravity; }

    void ApplyForce(Vector2D F);
    void ApplyForceX(float Fx);
    void ApplyForceY(float Fy);

    inline void SetForce(Vector2D force) { m_force = force; }

    inline void UnSetForce() { m_force = Vector2D(0, 0); }

    inline void SetFriction(Vector2D Fr) { m_friction = Fr; }

    inline void UnSetFriction() { m_friction = Vector2D(0, 0); }

    inline void ApplyVelocity(Vector2D v) { m_velocity = m_velocity + v; }

    inline void SetVelocity(Vector2D v) { m_velocity = v; }

    inline void MovePosition(Vector2D v) { m_position = m_position + v; }

    inline void SetPosition(Vector2D v) { m_position = v; }

    inline void SetPositionX(float x) { m_position.X = x; }

    inline void SetPositionY(float y) { m_position.Y = y; }

    inline float GetMass() const { return m_mass; }

    inline Vector2D GetForce() const { return m_force; }

    inline Vector2D GetFriction() const { return m_friction; }

    [[nodiscard]] inline Vector2D Position() const { return m_position; }

    inline Vector2D Velocity() const { return m_velocity; }

    inline Vector2D Acceleration() const { return m_acceleration; }

    void Update(float dt);

    void UpdateProjectile(float dt);

   private:
    float m_mass;
    float m_gravity;

    Vector2D m_force;
    Vector2D m_friction;

    Vector2D m_position;
    Vector2D m_velocity;
    Vector2D m_acceleration;
};
