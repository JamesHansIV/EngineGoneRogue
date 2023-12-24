#pragma once

#include <SDL2/SDL.h>

class Collider;

enum class EventType {
    UserEvent = 0,
    CollideEvent,
    TargetFoundEvent,
    TargetLostEvent
};

class Event {
   public:
    bool handled{false};
    virtual ~Event() = default;
    virtual EventType GetEventType() = 0;
};

class UserEvent : public Event {
   public:
    explicit UserEvent() : m_Event(nullptr) {}

    void SetSDLEvent(SDL_Event* event) { m_Event = event; }

    SDL_Event* GetSDLEvent() { return m_Event; }

    virtual EventType GetEventType() { return EventType::UserEvent; }

   private:
    SDL_Event* m_Event;
};

class CollideEvent : public Event {
   public:
    explicit CollideEvent(Collider* collidee) : m_Collidee(collidee) {}

    void SetCollidee(Collider* collidee);

    Collider* GetCollidee();

    virtual EventType GetEventType() { return EventType::CollideEvent; }

   private:
    Collider* m_Collidee;
};

class TargetFoundEvent : public Event {
   public:
    explicit TargetFoundEvent(float targetX, float targetY)
        : m_TargetX(targetX), m_TargetY(targetY) {}

    virtual EventType GetEventType() { return EventType::TargetFoundEvent; }

   private:
    float m_TargetX;
    float m_TargetY;
};

class TargetLostEvent : public Event {
   public:
    explicit TargetLostEvent() {}

    virtual EventType GetEventtype() { return EventType::TargetLostEvent; }
};
