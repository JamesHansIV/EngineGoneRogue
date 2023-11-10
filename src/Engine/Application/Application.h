#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "Engine/Events/EventManager.h"
#include "Engine/Layers/Layer.h"
#include "Engine/Objects/Map.h"
#include <unordered_map>
#include <tinyxml2.h>

class Application{
    public:
        Application();
        virtual ~Application() {}

        bool LoadTextures(char* projectPath);
        bool LoadObject(tinyxml2::XMLElement* xmlObj, std::string roomID);
        bool LoadObjects(std::string roomPath, std::string roomID);
        bool LoadRooms(char* projectPath);
        bool LoadProject();

        bool Clean();
        void Quit();

        virtual void Run();
        virtual void Update(float dt) {}
        virtual void Render() {}
        virtual void Events();

        inline bool IsRunning(){return m_IsRunning;}
        inline SDL_Window* GetWindow() { return m_Window; }
        inline EventManager& GetEventManager() { return m_EventManager; }
        inline std::string GetProjectName() { return m_ProjectName; }
        inline static Application* Get() { return m_Instance; }

    protected:
        std::string m_ProjectName;
        std::unordered_map<std::string, std::vector<GameObject*>> m_Rooms;
    private:
        bool m_IsRunning;
        SDL_Window* m_Window;
        EventManager m_EventManager;
        static Application* m_Instance;
};
