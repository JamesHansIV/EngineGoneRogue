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
    GameOverEvent,
    PlaceChestIfNeededEvent,
    ChestOpenedEvent,

    StartGameEvent,
    LevelUpSelectedGameEvent,
    ContinueGameEvent,
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

class TargetLostEvent : public Event {
   public:
    explicit TargetLostEvent() = default;

    EventType GetEventType() override { return EventType::TargetLostEvent; }
};

class PlaceChestIfNeededEvent : public Event {
   public:
    explicit PlaceChestIfNeededEvent(float x, float y) : m_x(x), m_y(y){};

    EventType GetEventType() override {
        return EventType::PlaceChestIfNeededEvent;
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
