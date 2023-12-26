#include "Engine/Objects/Entrance.h"
#include "State.h"

class EntranceState : public State {
   public:
    explicit EntranceState(Entrance* entrance) : m_Entrance(entrance) {}

    Entrance* GetEntrance() { return m_Entrance; }

   private:
    Entrance* m_Entrance;
};

class EntranceOpened : public EntranceState {
   public:
    explicit EntranceOpened(Entrance* entrance) : EntranceState(entrance) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Opened; }
};

class EntranceClosed : public EntranceState {
   public:
    explicit EntranceClosed(Entrance* entrance) : EntranceState(entrance) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    State* OnCollideEvent(CollideEvent* event);

    StateType GetType() override { return StateType::Closed; }
};

class EntranceOpening : public EntranceState {
   public:
    explicit EntranceOpening(Entrance* entrance) : EntranceState(entrance) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Opening; }
};

class EntranceClosing : public EntranceState {
   public:
    explicit EntranceClosing(Entrance* entrance) : EntranceState(entrance) {}

    void Enter() override;
    void Exit() override;
    State* Update(float dt) override;
    void Draw() override;
    State* HandleEvent(Event* event) override;

    StateType GetType() override { return StateType::Closing; }
};
