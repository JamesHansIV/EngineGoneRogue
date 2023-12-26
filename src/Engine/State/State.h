#pragma once

#include "Engine/Events/Event.h"

enum class GlobalState {
    None = 0,
    GameOver,
    Pause,
    Combat,
    LowHealth,
    ClearedLevel,
    ClearedRoom
};

enum class StateType {
    //General state
    Idle,
    IsHit,
    Moving,

    //Character state
    Dead,
    Attack,

    //Entrance state
    Closed,
    Opened,
    Closing,
    Opening
};

class BitFieldState {
   public:
    BitFieldState() : m_CurrentStates(0) {}

    int GetCurrentStates() const { return m_CurrentStates; }

    void SetState(int state) { m_CurrentStates = state; }

    void ToggleState(int state) { m_CurrentStates ^= state; }

    void AddState(int state) { m_CurrentStates |= state; }

    void RemoveState(int state) { m_CurrentStates &= ~state; }

    bool HasState(int state) const { return m_CurrentStates & state; }

   private:
    int m_CurrentStates;
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
