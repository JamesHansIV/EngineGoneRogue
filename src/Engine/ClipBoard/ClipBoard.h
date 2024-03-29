#pragma once

#include "Engine/Objects/GameObject.h"

#include <vector>

class ClipBoard {
    private:
    std::vector<GameObject*> m_objects;

    public:
    ClipBoard(){};
    ~ClipBoard();

    void AddObject(GameObject* object);
    void AddObjects(std::vector<GameObject*> objects);
    void Clear();

    int Size() { return m_objects.size(); }
    bool Empty() { return m_objects.empty(); }

    std::vector<GameObject*> GetObjects() { return m_objects; }
};