#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "../Events/EventManager.h"
#include "Layer.h"
#include <unordered_map>

class Engine{
    public:
        static Engine* GetInstance() 
        {
            return m_Instance = (m_Instance != nullptr) ? m_Instance : new Engine();
        }

        bool Init();
        bool Clean();
        void Quit();

        void Update(float dt);
        void Render();
        void Events();

        void AddObject(GameObject* obj) { m_ObjectStore[m_CurrObjectID] = obj; m_CurrObjectID++; }
        GameObject* GetObject(int id) { return m_ObjectStore[id]; }
        void AddComponent(Component* component);
        std::vector<Component*>& GetComponents(ComponentType type);

        int GetCurrentObjID() { return m_CurrObjectID; }

        inline bool IsRunning(){return m_IsRunning;};
        inline SDL_Renderer* GetRenderer(){return m_Renderer;};

    
    private:
        Engine(){};
        bool m_IsRunning;
    
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
        EventManager m_EventHandler;
        //Map* m_Map;

        int m_CurrObjectID;
        std::unordered_map<int, GameObject*> m_ObjectStore;
        std::unordered_map<ComponentType, std::vector<Component*>> m_ComponentStore;
        std::vector<Layer*> layers;
        static Engine* m_Instance;
};