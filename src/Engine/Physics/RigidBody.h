#pragma once
#include "Vector2D.h"

#define UNI_MASS 1.0f
#define GRAVITY 9.8f

class RigidBody {
    public:
        explicit RigidBody(float x, float y, float mass = UNI_MASS) {
            m_Mass = mass;
            m_Gravity = GRAVITY;
            m_Position = Vector2D(x, y);
        }

        inline void SetMass(float mass){m_Mass = mass;}
        inline void SetGravity(float gravity){m_Gravity = gravity;}

        void ApplyForce(Vector2D F);
        void ApplyForceX(float Fx);
        void ApplyForceY(float Fy);
        inline void SetForce(Vector2D force) { m_Force = force; }
        inline void UnSetForce(){m_Force = Vector2D(0,0);}

        inline void SetFriction(Vector2D Fr){m_Friction = Fr;}
        inline void UnSetFriction(){m_Friction = Vector2D(0,0);}

        inline void ApplyVelocity(Vector2D v) { m_Velocity = m_Velocity + v; }
        inline void SetVelocity(Vector2D v) { m_Velocity = v; }

        inline void MovePosition(Vector2D v) { m_Position = m_Position + v; }
        inline void SetPosition(Vector2D v) { m_Position = v; }
        inline void SetPositionX(float x) { m_Position.X = x; }
        inline void SetPositionY(float y) { m_Position.Y = y; }

        inline float GetMass() const {return m_Mass;}
        inline Vector2D GetForce() const { return m_Force; }
        inline Vector2D GetFriction() const { return m_Friction; }
        inline Vector2D Position() const {return m_Position;}
        inline Vector2D Velocity() const {return m_Velocity;}
        inline Vector2D Acceleration() const {return m_Acceleration;}

        void Update(float dt);

        void UpdateProjectile(float dt);

    private:
        float m_Mass;
        float m_Gravity;

        Vector2D m_Force;
        Vector2D m_Friction;

        Vector2D m_Position;
        Vector2D m_Velocity;
        Vector2D m_Acceleration;

};
