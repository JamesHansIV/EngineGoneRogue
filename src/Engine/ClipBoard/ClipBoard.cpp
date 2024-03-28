#include "ClipBoard.h"

ClipBoard::~ClipBoard() {
    for (auto& obj : m_objects)
        delete obj;
    
    m_objects.clear();
}

void ClipBoard::AddObject(GameObject* object) {
    m_objects.push_back(object);
}

void ClipBoard::AddObjects(std::vector<GameObject*> objects) {
    for (auto& obj : objects)
        m_objects.push_back(obj);
}

void ClipBoard::Clear() {
    m_objects.clear();
}