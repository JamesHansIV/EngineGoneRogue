#include <vector>
#include "Engine/Objects/GameObject.h"
#include "Events/EventListener.h"

class Layer : public EventListener {
    public:
        virtual ~Layer() {}
        virtual void OnAttach() = 0;
        virtual void OnDetach() = 0;
        virtual void Update() = 0;
        virtual void OnEvent(Event& event) = 0;
        void AddObject(GameObject* obj) { objects.push_back(obj); }
        void RemoveEntity(GameObject* obj);
    private:
        std::vector<GameObject*> objects;
};