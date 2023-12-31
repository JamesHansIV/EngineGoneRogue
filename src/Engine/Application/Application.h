#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <tinyxml2.h>
#include <unordered_map>
#include "Engine/Layers/Layer.h"
#include "Engine/Objects/Collider.h"

class Application {
   public:
    Application();
    virtual ~Application() = default;

    static bool LoadTextures(char* projectPath);
    static GameObject* LoadObject(tinyxml2::XMLElement* xmlObj);
    static Collider* LoadCollider(tinyxml2::XMLElement* xmlObj,
                                  GameObject* obj);
    bool LoadObjects(const std::string& roomPath, const std::string& roomID);
    bool LoadRooms(const char* projectPath);
    bool LoadProject();

    bool Clean();
    void Quit();

    virtual void Run();

    virtual void Update(float dt) {}

    virtual void Render() {}

    virtual void Events();

    inline int GetFrame() { return m_Frame; }

    inline bool IsRunning() const { return m_IsRunning; }

    inline SDL_Window* GetWindow() { return m_Window; }

    inline std::string GetProjectName() { return m_ProjectName; }

    inline static Application* Get() { return m_instance; }

   protected:
    std::string m_ProjectName;
    std::unordered_map<std::string, std::vector<GameObject*>> m_Rooms;
    float m_LastTick;
    int m_Frame;

   private:
    bool m_IsRunning;
    SDL_Window* m_Window;
    static Application* m_instance;
};
