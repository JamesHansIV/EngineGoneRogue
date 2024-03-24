#include "Application.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/GameObject.h"

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

Application::Application() : m_project_name("test_project") {
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

bool Application::LoadCharacters(const char* projectPath) {
    std::string objects_path = projectPath;
    objects_path += "/objects.xml";

    std::vector<GameObject*> const objects = LoadObjects(objects_path.c_str());

    if (objects.empty()) {
        return false;
    }

    for (auto* obj : objects) {
        if (obj->GetID() == "player") {
            m_player = static_cast<Player*>(obj);
            continue;
        }
        SDL_Log("adding obj %s", obj->GetID().c_str());
        m_rooms[m_base_room_id].push_back(obj);
    }

    return true;
}

bool Application::LoadRooms(const char* projectPath) {
    struct dirent* entry;
    DIR* dp;

    std::string rooms_path = projectPath;
    rooms_path += "/rooms";
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

bool Application::LoadProject() {
    char project_path[FilepathLen + 1];
    snprintf(project_path, FilepathLen, "../assets/projects/%s",
             m_project_name.c_str());
    if (!LoadTextures(project_path)) {
        return false;
    }
    SDL_Log("Textures are fine");
    SDL_Log("%s", project_path);
    if (!LoadCharacters(project_path)) {
        SDL_Log("cannot load characters");
        return false;
    }
    return LoadRooms(project_path);
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

        SDL_Log("current time: %f", new_time);
        SDL_Log("current time: %d", timer.GetCurrentTime());

        SDL_Log("Frame time: %f", frame_time);

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

bool Application::Clean() {
    Renderer::GetInstance()->Destroy();
    delete Renderer::GetInstance();

    for (const auto& it : m_rooms) {
        if (it.first == m_base_room_id) {
            continue;
        }
        for (auto* obj : it.second) {
            delete obj;
        }
    }

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
