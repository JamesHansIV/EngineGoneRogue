#include "ActionRecord.h"

ActionRecord::ActionRecord(EditorAction action, GameObject* object, int layer) {
    m_action = action;
    m_layer = layer;
    m_objects.emplace_back(object);
}

ActionRecord::ActionRecord(EditorAction action, std::vector<GameObject*> objects, int layer) {
    m_action = action;
    m_layer = layer;
    for (auto& obj : objects)
        m_objects.emplace_back(obj);
}

ActionRecord::~ActionRecord() {
    for (auto& obj : m_objects)
        delete obj;
    m_objects.clear();
}

void ActionRecord::SetObject(GameObject* object, int pos) {
    if(pos >= m_objects.size()) {
        m_objects.emplace_back(object);
    } else {
        m_objects.at(pos) = object;
    }
}