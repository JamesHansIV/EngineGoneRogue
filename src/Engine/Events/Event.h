#pragma once

#include <SDL2/SDL.h>
#include <string>
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
    GameOverEvent,
    PlaceItemIfNeededEvent,
    ChestOpenedEvent,

    StartGameEvent,
    LevelUpSelectedGameEvent,
    ContinueGameEvent,
    RestartGameEvent,
    RoomTransitionEvent,
    RoomClearEvent,

    MouseButtonDownEvent,
    EscapeKeyPressedEvent,
    WindowFocusLostEvent,
};

class Event {
   public:
    bool handled{false};
    virtual ~Event() = default;

    virtual EventType GetEventType() { return EventType::UserEvent; }
};

class UserEvent : public Event {
   public:
    explicit UserEvent() = default;

    void SetSDLEvent(SDL_Event* event) { m_event = event; }

    SDL_Event* GetSDLEvent() { return m_event; }

    EventType GetEventType() override { return EventType::UserEvent; }

   private:
    SDL_Event* m_event{nullptr};
};

class CollideEvent : public Event {
   public:
    explicit CollideEvent(Collider* collidee) : m_collidee(collidee) {}

    void SetCollidee(Collider* collidee);

    Collider* GetCollidee();

    EventType GetEventType() override { return EventType::CollideEvent; }

   private:
    Collider* m_collidee;
};

class TargetFoundEvent : public Event {
   public:
    explicit TargetFoundEvent(float targetX, float targetY)
        : m_target_x(targetX), m_target_y(targetY) {}

    EventType GetEventType() override { return EventType::TargetFoundEvent; }

   private:
    float m_target_x;
    float m_target_y;
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

class PlayerLevelUpEvent : public Event {
   public:
    explicit PlayerLevelUpEvent() {}

    EventType GetEventType() override { return EventType::PlayerLevelUpEvent; }

   private:
};

class TargetLostEvent : public Event {
   public:
    explicit TargetLostEvent() = default;

    EventType GetEventType() override { return EventType::TargetLostEvent; }
};

class PlaceItemIfNeededEvent : public Event {
   public:
    explicit PlaceItemIfNeededEvent(float x, float y) : m_x(x), m_y(y){};

    EventType GetEventType() override {
        return EventType::PlaceItemIfNeededEvent;
    }

    [[nodiscard]] [[nodiscard]] float GetX() const { return m_x; };

    [[nodiscard]] [[nodiscard]] float GetY() const { return m_y; };

   private:
    float m_x;
    float m_y;
};

class ChestOpenedEvent : public Event {
   public:
    explicit ChestOpenedEvent(std::vector<ItemType>& itemTypes,
                              std::pair<float, float>& index)
        : m_item_types(itemTypes), m_index(index) {}

    EventType GetEventType() override { return EventType::ChestOpenedEvent; };

    std::vector<ItemType> GetItemTypes() { return m_item_types; };

    std::pair<float, float> GetIndex() { return m_index; };

   private:
    std::vector<ItemType> m_item_types;
    std::pair<float, float> m_index;
};

class StartGameEvent : public Event {
   public:
    explicit StartGameEvent() = default;

    EventType GetEventType() override { return EventType::StartGameEvent; }

   private:
};

class ContinueGameEvent : public Event {
   public:
    explicit ContinueGameEvent() = default;

    EventType GetEventType() override { return EventType::ContinueGameEvent; }

   private:
};

class GameOverEvent : public Event {
   public:
    explicit GameOverEvent() = default;

    EventType GetEventType() override { return EventType::GameOverEvent; }

   private:
};

class RestartGameEvent : public Event {
   public:
    explicit RestartGameEvent() = default;

    EventType GetEventType() override { return EventType::RestartGameEvent; }

   private:
};

class RoomTransitionEvent : public Event {
   public:
    explicit RoomTransitionEvent(std::string next_room_id)
        : m_next_room_id(next_room_id) {}

    std::string GetNextRoomID() { return m_next_room_id; }

    EventType GetEventType() override { return EventType::RoomTransitionEvent; }

   private:
    std::string m_next_room_id;
};

class RoomClearEvent : public Event {
   public:
    explicit RoomClearEvent() {}

    EventType GetEventType() override { return EventType::RoomClearEvent; }

   private:
};

class LevelUpSelectedGameEvent : public Event {
   public:
    explicit LevelUpSelectedGameEvent() {}

    EventType GetEventType() override {
        return EventType::LevelUpSelectedGameEvent;
    }

   private:
};

class MouseButtonDownEvent : public Event {
   public:
    explicit MouseButtonDownEvent(int x, int y, int type)
        : m_x(x), m_y(y), m_type(type) {}

    int GetX() { return m_x; }

    int GetY() { return m_y; }

    int GetType() { return m_type; }

    EventType GetEventType() override {
        return EventType::MouseButtonDownEvent;
    }

   private:
    int m_x;
    int m_y;
    int m_type;
};

class EscapeKeyPressedEvent : public Event {
   public:
    explicit EscapeKeyPressedEvent() {}

    EventType GetEventType() override {
        return EventType::EscapeKeyPressedEvent;
    }

   private:
};

class WindowFocusLostEvent : public Event {
   public:
    explicit WindowFocusLostEvent() {}

    EventType GetEventType() override {
        return EventType::WindowFocusLostEvent;
    }

   private:
};
