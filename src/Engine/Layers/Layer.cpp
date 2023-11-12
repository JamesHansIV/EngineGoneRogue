#include "Layer.h"

void Layer::RemoveEntity(GameObject* obj) {

    for (int i = 0; i < m_objects.size(); i++) {
        if (obj->GetID() == m_objects[i]->GetID()) {
            m_objects.erase(m_objects.begin() + i);
        }
    }
}