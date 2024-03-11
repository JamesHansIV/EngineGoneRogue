#include "RigidBody.h"

void RigidBody::Update(float dt) {
    m_force = m_force - m_friction;
    // m_Acceleration.X = m_Force.X/m_Mass;
    // m_Acceleration.Y = m_Force.Y/m_Mass;
    m_acceleration = m_velocity * -0.2;
    m_velocity = m_velocity + m_acceleration * dt * 25;
    if (-0.01 < m_velocity.X && m_velocity.X < 0.01) {
        m_velocity.X = 0;
    }
    if (-0.01 < m_velocity.Y && m_velocity.Y < 0.01) {
        m_velocity.Y = 0;
    }
    m_position = m_position + m_velocity * dt * 10;
}

void RigidBody::ApplyForce(Vector2D f) {
    m_force = m_force + f;
    m_friction = m_force * 0.2;
}

void RigidBody::ApplyForceX(float fx) {
    m_force.X = m_force.X + fx;
    m_friction.X = m_friction.X + m_force.X * 0.2;
}

void RigidBody::ApplyForceY(float fy) {
    m_force.Y = m_force.Y + fy;
    m_friction.X = m_friction.X + m_force.Y * 0.2;
}
