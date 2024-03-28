#pragma once

#include "Engine/Events/Event.h"

enum class GlobalState {
    None = 0,
    Running,
    Start,
    GameOver,
    Pause,
    Shop,
    Combat,
    LowHealth,
    ClearedLevel,
    ClearedRoom
};

enum class StateType {
    //Global state
    Running,
    Start,
    GameOver,
    LevelUp,
    Pause,
    ChestDrop,
    Shop,
    RoomTransition,

    //General state
    Idle,
    IsHit,
    Moving,

    //Character state
    Dead,
    Attack,
    Dodge,

    //Entrance state
    Closed,
    Opened,
    Closing,
    Opening
};

class BitFieldState {
   public:
    BitFieldState() = default;

    [[nodiscard]] int GetCurrentStates() const { return m_current_states; }

    void SetState(int state) { m_current_states = state; }

    void ToggleState(int state) { m_current_states ^= state; }

    void AddState(int state) { m_current_states |= state; }

    void RemoveState(int state) { m_current_states &= ~state; }

    [[nodiscard]] bool HasState(int state) const {
        return (m_current_states & state) != 0;
    }

   private:
    int m_current_states{0};
};

class State {
   public:
    virtual ~State() = default;

    virtual void Enter() = 0;
    virtual void Exit() = 0;
    virtual State* Update(float dt) = 0;
    virtual void Draw() = 0;
    virtual State* HandleEvent(Event* event) = 0;
    virtual StateType GetType() = 0;
};
