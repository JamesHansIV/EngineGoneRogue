#pragma once

#include <SDL2/SDL.h>
#include <tuple>
#include <vector>
#include "Engine/Objects/Characters/EnemyStats.h"

class Collider;

enum class ItemType;

enum class EventType {
    UserEvent = 0,
    CollideEvent,
    TargetFoundEvent,
    TargetLostEvent,
    EnemyDeathEvent,
    PlayerLevelUpEvent,
    PlaceChestIfNeededEvent,
    ChestOpenedEvent,

    StartGameEvent,
    MouseDownEvent,
    ButtonDownEvent
};

class Event {
   public:
    bool handled{false};
    virtual ~Event() = default;

    virtual EventType GetEventType() { return EventType::UserEvent; }
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

class EnemyDeathEvent : public Event {
   public:
    explicit EnemyDeathEvent(const EnemyStats& enemy_stats) {
        m_stats = EnemyStats(enemy_stats);
    }

    [[nodiscard]] EnemyStats GetEnemyStats() const { return m_stats; }

    EventType GetEventType() override { return EventType::EnemyDeathEvent; }

   private:
    EnemyStats m_stats;
};

class TargetLostEvent : public Event {
   public:
    explicit TargetLostEvent() {}

    virtual EventType GetEventType() { return EventType::TargetLostEvent; }
};

class PlaceChestIfNeededEvent : public Event {
   public:
    explicit PlaceChestIfNeededEvent(float x, float y) : m_X(x), m_Y(y){};

    virtual EventType GetEventType() override {
        return EventType::PlaceChestIfNeededEvent;
    }

    float GetX() { return m_X; };

    float GetY() { return m_Y; };

   private:
    float m_X;
    float m_Y;
};

class ChestOpenedEvent : public Event {
   public:
    explicit ChestOpenedEvent(std::vector<ItemType>& itemTypes,
                              std::pair<float, float>& index)
        : m_ItemTypes(itemTypes), m_Index(index) {}

    virtual EventType GetEventType() override {
        return EventType::ChestOpenedEvent;
    };

    std::vector<ItemType> GetItemTypes() { return m_ItemTypes; };

    std::pair<float, float> GetIndex() { return m_Index; };

   private:
    std::vector<ItemType> m_ItemTypes;
    std::pair<float, float> m_Index;
};

class PlayerLevelUpEvent: public Event {
    public:
    explicit PlayerLevelUpEvent(){};
    virtual EventType GetEventType() override {
        return EventType::PlayerLevelUpEvent;
    };
    private:
};

class StartGameEvent : public Event {
   public:
    explicit StartGameEvent() {}

    virtual EventType GetEventType() override {
        return EventType::StartGameEvent;
    }

   private:
};

class MouseDownEvent : public Event {
   public:
    explicit MouseDownEvent(int x, int y, int button)
        : m_X(x), m_Y(y), m_Button(button) {}

    virtual EventType GetEventType() override {
        return EventType::MouseDownEvent;
    }

    int GetX() { return m_X; }

    int GetY() { return m_Y; }

    int GetButton() { return m_Button; }

   private:
    int m_X;
    int m_Y;
    int m_Button;
};

class ButtonDownEvent : public Event {
    public:
    explicit ButtonDownEvent(int button)
        : m_Button(button) {}

    virtual EventType GetEventType() override {
        return EventType::ButtonDownEvent;
    }

    int GetButton() { return m_Button; }

   private:
    int m_Button;
};
