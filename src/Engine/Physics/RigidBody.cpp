#include "RigidBody.h"

void RigidBody::Update(float dt)
{
    if (-0.3 < m_Force.X && m_Force.X < 0.3) {
        m_Force.X = 0;
        m_Friction.X = 0;
    }
    if (-0.3 < m_Force.Y && m_Force.Y < 0.3) {
        m_Force.Y = 0;
        m_Friction.Y = 0;
    }
    m_Force = m_Force - m_Friction;
    // m_Acceleration.X = m_Force.X/m_Mass;
    // m_Acceleration.Y = m_Force.Y/m_Mass;
    m_Acceleration = m_Velocity * -0.2;
    m_Velocity = m_Velocity + m_Acceleration*dt;
    m_Position = m_Position + m_Velocity*dt;
}

void RigidBody::ApplyForce(Vector2D f) {
    m_Force = m_Force + f;
    m_Friction = m_Force * 0.2;
}

void RigidBody::ApplyForceX(float fx) {
    m_Force.X = m_Force.X + fx;
    m_Friction.X = m_Friction.X + m_Force.X * 0.2;
}

void RigidBody::ApplyForceY(float fy) {
    m_Force.Y = m_Force.Y + fy;
    m_Friction.X = m_Friction.X + m_Force.Y * 0.2;
}