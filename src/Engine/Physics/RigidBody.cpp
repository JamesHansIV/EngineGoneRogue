#include "RigidBody.h"

void RigidBody::Update(float dt)
{
    m_Acceleration.X = (m_Force.X - m_Friction.X)/m_Mass;
    m_Acceleration.Y = m_Force.Y/m_Mass;
    m_Velocity = m_Velocity + m_Acceleration*dt;
    m_Position = m_Position + m_Velocity*dt;
}