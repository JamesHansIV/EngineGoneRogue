#include "Application.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/GameObject.h"

#include "Engine/Input/InputChecker.h"

#include "Engine/Timer/Timer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Engine/Config/Config.h"
#include "Engine/utils/utils.h"

#include <dirent.h>
#include <unistd.h>

Application* Application::m_instance = nullptr;

//const float kDt = 0.0083;
const float kDt = 0.01;

Application::Application() : m_ProjectName("test_project"), m_Frame(0) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0 &&
        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        assert(false);
    }

    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize TTF: %s", TTF_GetError());
        assert(false);
    }

    m_Window = SDL_CreateWindow("Engine Gone Rogue", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, 0);
    if (m_Window == nullptr) {
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        assert(false);
    }

    m_instance = this;

    //TODO: note that the cwd is <projectDir>/build instead of <projectDir>.
    //      Set a working directory path macro to use absolute file paths
    Renderer::GetInstance()->Init();

    if (LOAD_PROJECT) {
        if (!LoadProject()) {
            SDL_Log("Failed to load project");
            assert(false);
        }
    }

    m_IsRunning = true;
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
            m_Rooms[room_id] = LoadObjects(room_path.c_str());
            room_path = rooms_path;
        }
    }

    closedir(dp);
    return true;
}

bool Application::LoadProject() {
    char project_path[FILEPATH_LEN + 1];
    snprintf(project_path, FILEPATH_LEN, "../assets/projects/%s",
             m_ProjectName.c_str());
    if (!LoadTextures(project_path)) {
        return false;
    }
    SDL_Log("Textures are fine");
    SDL_Log("%s", project_path);
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
    float accumulator = 0.0;
    int count_until_next_second = 1000;
    int fps = 0;
    int updates_per_second = 0;
    while (m_IsRunning) {
        Events();
        bool render = false;
        if (!timer.IsPaused()) {
            if (timer.GetTicks() > count_until_next_second) {
                SDL_Log("FPS: %d", fps);
                count_until_next_second += 1000;
                fps = 0;
                SDL_Log("Updates per second: %d", updates_per_second);
                updates_per_second = 0;
            }
            float const new_time = timer.GetTicks();
            auto frame_time = static_cast<float>(
                new_time / 1000.0F - timer.GetCurrentTime() / 1000.0F);
            timer.SetCurrentTime(new_time);
            // Necessary for slow computers
            // Todo: implement this if slowness ever becomes an issue.
            //if (frame_time > 0.25F) {
            //    frame_time = 0.25F;
            //}

            accumulator += frame_time;
            while (accumulator >= kDt) {
                Update(kDt);
                updates_per_second++;
                render = true;
                accumulator -= kDt;
            }
        }

        if (render) {
            m_Frame++;
            Render();
            fps++;
        }
    }
    Clean();
}

bool Application::Clean() {
    Renderer::GetInstance()->Destroy();
    delete Renderer::GetInstance();

    SDL_DestroyWindow(m_Window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return true;
}

void Application::Quit() {
    m_IsRunning = false;
}
