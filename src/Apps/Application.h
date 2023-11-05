#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "Events/EventManager.h"
#include "Layers/Layer.h"
#include "Engine/Objects/Map.h"
#include <unordered_map>

class Application{
    public:
        Application();
        virtual ~Application() {}
        
        bool Clean();
        void Quit();

        virtual void Run();
        virtual void Update(float dt) {}
        virtual void Render() {}
        virtual void Events();

        inline bool IsRunning(){return m_IsRunning;}
        inline SDL_Window* GetWindow() { return m_Window; }
        inline EventManager& GetEventManager() { return m_EventManager; }
        inline static Application* Get() { return m_Instance; }
    
    protected:
        std::string m_ProjectName;
    private:
        bool m_IsRunning;
        SDL_Window* m_Window;
        EventManager m_EventManager;
        static Application* m_Instance;
};