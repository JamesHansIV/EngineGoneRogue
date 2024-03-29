#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include <unordered_map>
#include "Engine/Audio/AudioManager.h"
#include "Engine/Events/EventManager.h"
#include "Engine/Layers/Layer.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Environment/Entrance.h"

class Application {
   public:
    Application();
    virtual ~Application() = default;

    bool LoadRooms();
    bool LoadNextRoom();
    bool LoadRoom(std::string room_id);
    bool LoadProject();
    bool LoadStart();
    bool LoadPlayer();
    bool BuildRoomIds();

    std::string GetProjectPath() {
        return "../assets/projects/" + m_project_name;
    }

    bool Clean();
    void ClearObjects();
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

    [[nodiscard]] AudioManager& GetAudioManager() { return m_audio_manager; }

    int& GetMutableWindowWidth() { return m_window_width; }

    int& GetMutableWindowHeight() { return m_window_height; }

    std::string GetStartID() { return m_start_room; }

    void SetStartPosition(int x, int y) { m_start_position = {x, y}; }

    std::pair<int, int> GetStartPosition() { return m_start_position; }

    std::pair<int, int> GetGlobalStart() { return m_global_start; }

    int GetEnemyCount() { return m_enemy_count; }

    void DecrementEnemyCount() {
        m_enemy_count--;
        assert(m_enemy_count >= 0);
    }

    void ResetNextRoom() { m_next_room = 0; }

    void IncrementEnemyCount() { m_enemy_count++; }

    void AddRoomCleared(std::string room_id) {
        m_rooms_cleared.insert(room_id);
    }

    Player* GetPlayer() { return m_player; }

    inline static Application& Get() { return *m_instance; }

   protected:
    std::string m_project_name;
    std::string m_base_room_id = "room6";
    std::string m_start_room;
    std::unordered_map<std::string, std::vector<GameObject*>> m_rooms;
    std::vector<GameObject*> m_objects;
    std::vector<GameObject*> m_tiles;
    std::vector<std::string> m_room_order;
    std::vector<std::string> m_room_ids;
    std::set<std::string> m_rooms_cleared;

    int m_next_room;
    std::pair<int, int> m_global_start;
    std::pair<int, int> m_start_position;
    int m_enemy_count;

    Uint32 m_last_tick;
    bool m_is_paused;
    bool m_has_focus;
    int m_window_width = 0;
    int m_window_height = 0;
    int m_frame{0};
    EventManager m_event_manager;
    Player* m_player;
    AudioManager m_audio_manager;

   private:
    bool m_is_running;
    SDL_Window* m_window;
    static Application* m_instance;
};
