#include "EntranceState.h"

void EntranceOpened::Enter() {
    GetEntrance()->GetAnimation()->SelectAnimation("close");
}

void EntranceOpened::Exit() {}

State* EntranceOpened::Update(float  /*dt*/) {
    return nullptr;
}

void EntranceOpened::Draw() {
    GetEntrance()->DrawAnimation();
}

State* EntranceOpened::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }
    return nullptr;
}

State* EntranceOpened::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            return new EntranceClosing(GetEntrance());
        default:
            break;
    }
    return nullptr;
}

void EntranceClosed::Enter() {
    GetEntrance()->GetAnimation()->SelectAnimation("open");
}

void EntranceClosed::Exit() {}

State* EntranceClosed::Update(float  /*dt*/) {
    return nullptr;
}

void EntranceClosed::Draw() {
    GetEntrance()->DrawAnimation();
}

State* EntranceClosed::HandleEvent(Event* event) {
    EventType const e_type = event->GetEventType();

    switch (e_type) {
        case EventType::CollideEvent:
            return OnCollideEvent(dynamic_cast<CollideEvent*>(event));
        default:
            break;
    }
    return nullptr;
}

State* EntranceClosed::OnCollideEvent(CollideEvent* event) {
    Collider* collidee = event->GetCollidee();

    if (collidee == nullptr) {
        return nullptr;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:

            SDL_Log("entrance collide with player");
            return new EntranceOpening(GetEntrance());
        default:
            break;
    }
    return nullptr;
}

void EntranceOpening::Enter() {
    SDL_Log("entrance is opening");
    GetEntrance()->GetAnimation()->SelectAnimation("open");
}

void EntranceOpening::Exit() {}

State* EntranceOpening::Update(float  /*dt*/) {
    GetEntrance()->GetAnimation()->Update();

    if (GetEntrance()->GetAnimation()->Ended()) {
        return new EntranceOpened(GetEntrance());
    }
    return nullptr;
}

void EntranceOpening::Draw() {
    GetEntrance()->DrawAnimation();
}

State* EntranceOpening::HandleEvent(Event*  /*event*/) {
    return nullptr;
}

void EntranceClosing::Enter() {
    GetEntrance()->GetAnimation()->SelectAnimation("close");
}

void EntranceClosing::Exit() {}

State* EntranceClosing::Update(float  /*dt*/) {
    GetEntrance()->GetAnimation()->Update();

    if (GetEntrance()->GetAnimation()->Ended()) {
        return new EntranceClosed(GetEntrance());
    }
    return nullptr;
}

void EntranceClosing::Draw() {
    GetEntrance()->DrawAnimation();
}

State* EntranceClosing::HandleEvent(Event*  /*event*/) {
    return nullptr;
}
