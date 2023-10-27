#pragma once

#include "../Components/Component.h"
#include "../Components/Transform.h"
#include "../Components/DrawData.h"
#include <string>
#include <unordered_map>

class GameObject {
    public:
        GameObject(int objectID) : m_ObjectID(objectID) {
        }

        void AddComponent(Component* component) { m_Components[component->GetComponentType()] = component; }
        Component* GetComponent(ComponentType type) { assert(m_Components.find(type) != m_Components.end()); return m_Components[type]; }
        void Clean() {
            for (auto it = m_Components.begin(); it != m_Components.end(); ++it) {
                delete it->second;
            }
            m_Components.clear();
        }

        int GetID() { return m_ObjectID; }
        void SetID(int id) { m_ObjectID = id; }

    protected:
        int m_ObjectID;
        std::unordered_map<ComponentType, Component*> m_Components;
};