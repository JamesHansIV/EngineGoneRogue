#include "Application.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Projectiles/ProjectileManager.h"

#include "Engine/Input/InputChecker.h"

#include "Engine/Timer/Timer.h"
#include "SDL2/SDL_timer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Engine/Config/Config.h"
#include "Engine/utils/utils.h"

#include <SDL2_mixer/SDL_mixer.h>
#include <dirent.h>
#include <unistd.h>

Application* Application::m_instance = nullptr;

//const float kDt = 0.0083;
const float kDt = 0.01;

Application::Application()
    : m_project_name("test_project"),
      m_player(nullptr),
      m_next_room(0),
      m_start_position({0, 0}),
      m_enemy_count(0) {
    // std::cout << "DEBUG MESSAGE FLAG " << DEBUG_MESSAGES << std::endl;
    // SDL_Log("Something going on");
    // exit(0);
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0 &&
        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        assert(false);
    }

    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize TTF: %s", TTF_GetError());
        assert(false);
    }

    m_window = SDL_CreateWindow("Engine Gone Rogue", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, ScreenWidth,
                                ScreenHeight, SDL_WINDOW_RESIZABLE);
    if (m_window == nullptr) {
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        assert(false);
    }

    m_instance = this;

    SDL_GetWindowSize(m_window, &m_window_width, &m_window_height);

    //TODO: note that the cwd is <projectDir>/build instead of <projectDir>.
    //      Set a working directory path macro to use absolute file paths
    Renderer::GetInstance()->Init();

    if (ShouldLoadProject != 0U) {
        if (!LoadProject()) {
            SDL_Log("Failed to load project");
            assert(false);
        }
    }

    m_is_running = true;
}

bool Application::LoadRooms() {
    struct dirent* entry;
    DIR* dp;

    std::string rooms_path = GetProjectPath();
    rooms_path += "/rooms";

    // NOTE: comment these out to load all rooms for the editor
    std::string base_room_path = rooms_path + "/" + m_base_room_id + ".xml";
    m_tiles = LoadObjects(base_room_path.c_str());

    return true;

    dp = opendir(rooms_path.c_str());
    if (dp == nullptr) {
        perror("Rooms path does not exist");
        return false;
    }

    std::string room_path = rooms_path;
    std::string room_id;
    int const index = 0;
    while ((entry = readdir(dp)) != nullptr) {
        if (strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0) {
            std::string const file_name = entry->d_name;
            room_path += "/";
            room_path += entry->d_name;
            room_id = file_name.substr(0, file_name.rfind('.'));
            std::vector<GameObject*> objects = LoadObjects(room_path.c_str());
            m_rooms[room_id].insert(m_rooms[room_id].begin(), objects.begin(),
                                    objects.end());
            room_path = rooms_path;
        }
    }

    SDL_Log("room %s has size %d", room_id.c_str(), m_rooms[room_id].size());

    closedir(dp);
    return true;
}

bool Application::LoadNextRoom() {
    if (m_next_room == m_room_order.size()) {
        SDL_Log("Last room already loaded");
        PushNewEvent(EventType::GameOverEvent);
        return true;
    }
    ProjectileManager::GetInstance()->DeleteProjectiles();
    ColliderHandler::GetInstance()->Clear();
    LoadRoom(m_room_order[m_next_room].c_str());
    m_player->GetRigidBody()->SetPosition(
        Vector2D(m_start_position.first, m_start_position.second));
    m_next_room++;
    return true;
}

bool Application::LoadRoom(std::string room_id) {
    ClearObjects();

    std::string tile_path = GetProjectPath() + "/rooms/" + room_id + ".xml";

    // std::cout << "tile path: " << tile_path << "\n";
    m_tiles = LoadObjects(tile_path.c_str());
    // std::cout << "m_tiles.size " << m_tiles.size() << std::endl;

    if (m_tiles.empty()) {
        SDL_Log("m_tiles.empty(): Could not load %s", tile_path.c_str());
        return false;
    }

    std::string obj_path =
        GetProjectPath() + "/rooms/" + room_id + "_objects.xml";
    // std::cout << "objects path: " << tile_path << "\n";

    std::vector<GameObject*> const objects = LoadObjects(obj_path.c_str());
    // std::cout << "objects.size " << objects.size() << std::endl;


    if (objects.empty()) {
        SDL_Log("objects.empty(): Could not load %s", obj_path.c_str());
        return false;
    }

    Entrance* entrance = nullptr;
    for (auto* obj : objects) {
        SDL_Log("adding obj %s", obj->GetID().c_str());
        SDL_Log("object is collider: %d",
                dynamic_cast<Collider*>(obj) != nullptr);
        m_objects.push_back(static_cast<Collider*>(obj));
    }

    std::string start_pos_path =
        GetProjectPath() + "/rooms/" + room_id + ".start";

    if (!LoadStart(start_pos_path.c_str())) {
        SDL_Log("load start position failed");
        return false;
    }

    return true;
}

bool Application::LoadPlayer() {
    std::string path = GetProjectPath() + "/player.xml";

    SDL_Log("player path: %s", path.c_str());

    std::vector<GameObject*> objects = LoadObjects(path.c_str());

    if (objects.size() != 1) {
        SDL_Log("Failed to load player");
        return false;
    }

    m_player = static_cast<Player*>(objects[0]);
}

bool Application::LoadMap() {
    m_room_order = LoadRoomOrder(GetProjectPath().c_str());
    if (m_room_order.empty()) {
        return false;
    }
    return true;
}

bool Application::LoadStart(const char* path) {
    m_start_position = LoadStartPosition(path);
    return m_start_position.first != 0 || m_start_position.second != 0;
}

bool Application::BuildRoomIds() {
    struct dirent* entry;
    DIR* dp;

    std::string rooms_path = GetProjectPath();
    rooms_path += "/rooms";

    dp = opendir(rooms_path.c_str());
    if (dp == nullptr) {
        perror("Rooms path does not exist");
        return false;
    }

    std::string room_path;
    std::string room_id;
    while ((entry = readdir(dp)) != nullptr) {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            std::string const file_name = entry->d_name;

            if (file_name.rfind(".xml") == std::string::npos)
                continue;

            if (file_name.rfind("_objects.xml") != std::string::npos)
                continue;

            room_path += "/";
            room_path += entry->d_name;
            room_id = file_name.substr(0, file_name.rfind(".xml"));

            m_room_ids.push_back(room_id);

            room_path = rooms_path;
        }
    }

    closedir(dp);
    return true;
}

bool Application::LoadProject() {
    if (!LoadTextures(GetProjectPath().c_str())) {
        return false;
    }
    SDL_Log("Textures are fine");

    if (!LoadMap()) {
        SDL_Log("failed to load room order");
        return false;
    }

    if (!LoadPlayer()) {
        return false;
    }

    if (!BuildRoomIds()) {
        SDL_Log("failed to build room id vector");
        return false;
    }

    // load first room ? 
    if (!m_room_ids.empty())
        return LoadRoom(m_room_ids.front());
    
    return true;


    // return LoadNextRoom();

    //if (!LoadCharacters()) {
    //    SDL_Log("failed to load characters");
    //    return false;
    //}

    //return LoadRooms();
}

void Application::Events() {
    SDL_Event event;
    SDL_Log("Event");
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            SDL_Log("Event type: %d", event.type);
            case SDL_QUIT:
                Quit();
                return;
            case SDL_KEYDOWN:
                InputChecker::SetKeyPressed(event.key.keysym.sym, true);
                break;
            case SDL_KEYUP:
                InputChecker::SetKeyPressed(event.key.keysym.sym, false);
                break;
            case SDL_MOUSEBUTTONDOWN:
                InputChecker::SetMouseButtonPressed(event.button.button, true);
                break;
            case SDL_MOUSEBUTTONUP:
                InputChecker::SetMouseButtonPressed(event.button.button, false);
                break;
            case SDL_MOUSEMOTION:
                InputChecker::UpdateMousePosition(event.motion.x,
                                                  event.motion.y);
                break;
            case SDL_MOUSEWHEEL:
                InputChecker::SetMouseWheelDirection(event.wheel.y);
                break;
            case SDL_WINDOWEVENT:
                break;
        }
    }
}

void Application::Run() {
    timer.Start();
    timer.Pause();
    double accumulator = 0.0;
    int count_until_next_second = 1000;
    int fps = 0;
    int updates_per_second = 0;
    while (m_is_running) {
        Events();
        bool render = false;
        if (SDL_GetTicks() > count_until_next_second) {
            SDL_Log("FPS: %d", fps);
            count_until_next_second += 1000;
            fps = 0;
            SDL_Log("Updates per second: %d", updates_per_second);
            updates_per_second = 0;
        }
        double const new_time = timer.GetAbsoluteTicks();
        auto frame_time = static_cast<double>(
            new_time / 1000.0F -
            static_cast<double>(timer.GetCurrentTime()) / 1000.0F);
        timer.SetCurrentTime(static_cast<Uint32>(new_time));

        //SDL_Log("current time: %f", new_time);
        //SDL_Log("current time: %d", timer.GetCurrentTime());

        //SDL_Log("Frame time: %f", frame_time);

        if (frame_time > 0.25F) {
            frame_time = 0.25F;
        }

        accumulator += frame_time;
        while (accumulator >= kDt) {
            Update(kDt);
            updates_per_second++;
            render = true;
            accumulator -= kDt;
        }

        if (render) {
            m_frame++;
            Render();
            fps++;
        }
    }

    Clean();
}

void Application::ClearObjects() {
    for (auto* tile : m_tiles) {
        delete tile;
    }
    for (auto* obj : m_objects) {
        delete obj;
    }
    m_tiles.clear();
    m_objects.clear();
}

bool Application::Clean() {
    SDL_Log("enemy count in app: %d", GetEnemyCount());
    Renderer::GetInstance()->Destroy();
    delete Renderer::GetInstance();

    delete m_player;
    for (const auto& it : m_rooms) {
        if (it.first == m_base_room_id) {
            continue;
        }
        for (auto* obj : it.second) {
            delete obj;
        }
    }
    ClearObjects();

    SDL_DestroyWindow(m_window);
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();

    return true;
}

void Application::Quit() {
    m_is_running = false;
}
