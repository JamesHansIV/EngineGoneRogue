#pragma once

#include <vector>
#include "Engine/Events/EventListener.h"
#include "Engine/Objects/GameObject.h"

class Layer : public EventListener {
   public:
    virtual ~Layer() = default;
    virtual void OnAttach() = 0;
    virtual void OnDetach() = 0;
    virtual void Update() = 0;
    void OnEvent(Event& event) override = 0;

    void AddObject(GameObject* obj) { m_objects.push_back(obj); }

    void RemoveEntity(GameObject* obj);

   private:
    std::vector<GameObject*> m_objects;
};
