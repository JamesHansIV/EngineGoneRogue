#include "Layer.h"

void Layer::RemoveEntity(GameObject* obj) {

    for (int i = 0; i < objects.size(); i++) {
        if (obj->GetID() == objects[i]->GetID()) {
            objects.erase(objects.begin() + i);
        }
    }
}