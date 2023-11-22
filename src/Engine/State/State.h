#pragma once

enum class GlobalState {
    None = 0,
    GameOver,
    Pause,
    Combat,
    LowHealth,
    ClearedLevel,
    ClearedRoom
};

struct ObjectState {
    enum {
        None = 0,
        Colliding,
        Destroyed,
        Damaged,
        IsHit
    };
};

struct PlayerState : ObjectState {
    enum {
        None = 0,
        Dashing,
    };
};


template <typename T>
class State {
    public:
        State(T state) : m_CurrentState(state) {}

        T GetCurrentState() { return m_CurrentState; }
        void SetState(T state) { m_CurrentState = state; }
    private:
        T m_CurrentState;
};