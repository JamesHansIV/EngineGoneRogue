#pragma once

#include <list>

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
        Colliding = 1 << 0,
        ToBeDestroyed = 1 << 1,
        Damaged = 1 << 2,
        IsHit = 1 << 3,
        Idle = 1 << 4
    };
};

struct EntranceState : ObjectState {
    enum {
        Open = 1 << 5,
        Close = 1 << 6
    };
};

struct CharacterState : ObjectState {
    enum {
        Dead = 1 << 7,
        MoveUp = 1 << 8,
        MoveDown = 1 << 9,
        MoveRight = 1 << 10,
        MoveLeft = 1 << 11,
        IdleX = 1 << 12,
        IdleY = 1 << 13
    };
};


class State {
    public:
        State() : m_CurrentStates(0) {}

        int GetCurrentStates() { return m_CurrentStates; }

        void SetState(int state) { m_CurrentStates = state; }
        void ToggleState(int state) { m_CurrentStates ^= state; }
        void AddState(int state) { m_CurrentStates |= state; }
        void RemoveState(int state) { m_CurrentStates &= ~state; }
        bool HasState(int state) { return m_CurrentStates & state; }
    private:
        int m_CurrentStates;
        std::vector<int> m_StateOrder;
};