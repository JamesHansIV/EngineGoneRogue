#include "GrenadeDrop.h"
#include "Engine/Objects/Characters/Character.h"
#include "Engine/Objects/Characters/Player.h"

void GrenadeDrop::OnCollide(Collider* collidee) {
    if (this == collidee) {
        return;
    }

    switch (collidee->GetObjectType()) {
        case ObjectType::Player:
            dynamic_cast<Player*>(collidee)->AddBomb();
            m_marked_for_deletion = true;
            break;
        default:
            break;
    }
}
