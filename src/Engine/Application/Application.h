#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <unordered_map>
#include "Engine/Events/EventManager.h"
#include "Engine/Layers/Layer.h"
#include "Engine/Objects/Characters/Player.h"

class Application {
   public:
    Application();
    virtual ~Application() = default;

    bool LoadCharacters(const char* projectPath);
    bool LoadRooms(const char* projectPath);
    bool LoadProject();

    bool Clean();
    void Quit();

    virtual void Run();

    virtual void Update(float dt) {}

    virtual void Render() {}

    virtual void Events();

    [[nodiscard]] inline int GetFrame() const { return m_Frame; }

    [[nodiscard]] inline bool IsRunning() const { return m_is_running; }

    inline SDL_Window* GetWindow() { return m_window; }

    inline std::string GetProjectName() { return m_ProjectName; }

    [[nodiscard]] int GetWindowWidth() const { return m_WindowWidth; }

    [[nodiscard]] int GetWindowHeight() const { return m_WindowHeight; }

    int& GetMutableWindowWidth() { return m_WindowWidth; }

    int& GetMutableWindowHeight() { return m_WindowHeight; }

    Player* GetPlayer() { return m_Player; }

    inline static Application* Get() { return m_instance; }

   protected:
    std::string m_ProjectName;
    std::string m_BaseRoomID = "room1";
    std::unordered_map<std::string, std::vector<GameObject*>> m_Rooms;
    Uint32 m_LastTick;
    bool m_is_paused;
    bool m_has_focus;
    int m_WindowWidth = 0;
    int m_WindowHeight = 0;
    int m_Frame;
    EventManager m_EventManager;
    Player* m_Player;

   private:
    bool m_is_running;
    SDL_Window* m_window;
    static Application* m_instance;
};
