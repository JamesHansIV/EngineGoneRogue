#include "Event.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/Item.h"

void CollideEvent::SetCollidee(Collider* collidee) {
    m_collidee = collidee;
}

Collider* CollideEvent::GetCollidee() {
    return m_collidee;
}
