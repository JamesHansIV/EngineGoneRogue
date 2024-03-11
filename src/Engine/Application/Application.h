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

    [[nodiscard]] inline int GetFrame() const { return m_frame; }

    [[nodiscard]] inline bool IsRunning() const { return m_is_running; }

    inline SDL_Window* GetWindow() { return m_window; }

    inline std::string GetProjectName() { return m_project_name; }

    [[nodiscard]] int GetWindowWidth() const { return m_window_width; }

    [[nodiscard]] int GetWindowHeight() const { return m_window_height; }

    int& GetMutableWindowWidth() { return m_window_width; }

    int& GetMutableWindowHeight() { return m_window_height; }

    Player* GetPlayer() { return m_player; }

    inline static Application* Get() { return m_instance; }

   protected:
    std::string m_project_name;
    std::string m_base_room_id = "room1";
    std::unordered_map<std::string, std::vector<GameObject*>> m_rooms;
    Uint32 m_last_tick;
    bool m_is_paused;
    bool m_has_focus;
    int m_window_width = 0;
    int m_window_height = 0;
    int m_frame;
    EventManager m_event_manager;
    Player* m_player;

   private:
    bool m_is_running;
    SDL_Window* m_window;
    static Application* m_instance;
};
