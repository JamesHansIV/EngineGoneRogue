#include "GameEventManager.h"
#include <random>
#include "Engine/Application/Application.h"
#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/Chests/Chest.h"
#include "Engine/Timer/Timer.h"

CustomEventType custom_event_type = SDL_RegisterEvents(1);

GameEventManager::GameEventManager(Player* player,
                                   std::vector<GameObject*>& objects)
    : m_player(player), m_Objects(objects) {}

void GameEventManager::HandleEvents() {
    SDL_Event event;
    UserEvent event_wrapper;
    event_wrapper.SetSDLEvent(&event);
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                Application::Get()->Quit();
                return;
            case SDL_KEYDOWN:
                InputChecker::SetKeyPressed(event.key.keysym.sym, true);
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        if (timer.IsPaused()) {
                            timer.Unpause();
                        } else {
                            timer.Pause();
                        }
                    default:
                        break;
                }
                //player->OnKeyPressed(event);
                break;
            case SDL_KEYUP:
                InputChecker::SetKeyPressed(event.key.keysym.sym, false);
                //player->OnKeyReleased(event);
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
                // TODO: Add pause menu to render when focus is lost
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    timer.Pause();
                }
                if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_GetWindowSize(
                        Application::Get()->GetWindow(),
                        &Application::Get()->GetMutableWindowWidth(),
                        &Application::Get()->GetMutableWindowHeight());

                    Renderer::GetInstance()->SetCameraSize(
                        Application::Get()->GetWindowWidth(),
                        Application::Get()->GetWindowHeight());
                }
                break;
            case SDL_USEREVENT:
                switch (static_cast<EventType>(event.user.code)) {
                    case EventType::UserEvent: {
                        // Todo: once UserEvent is implemented to use
                        // SDL_USEREVENT, uncomment this
                        // UserEvent user_event = UserEvent();
                        // user_event.SetSDLEvent(&event);
                        // event_wrapper = user_event;
                        break;
                    }
                    case EventType::CollideEvent:
                    case EventType::TargetFoundEvent:
                    case EventType::TargetLostEvent:
                        break;
                    case EventType::EnemyDeathEvent: {
                        auto* enemy = static_cast<Enemy*>(event.user.data1);
                        if (enemy == nullptr) {
                            SDL_Log(
                                "ERROR: Enemy from EnemyDeathEvent is null");
                        }
                        EnemyDeathEvent death_event(enemy->GetEnemyStats());
                        if (m_player != nullptr) {
                            m_player->HandleEvent(&death_event);
                            ChestDrops(enemy->GetX(), enemy->GetY());
                        }
                        return;
                    }
                    case EventType::PlayerLevelUpEvent:
                        timer.Pause();
                        break;
                    default:
                        break;
                }
            default:
                break;
        }
        if (timer.IsPaused()) {
            return;
        }
        if (m_player != nullptr) {
            m_player->HandleEvent(&event_wrapper);
        }
    }
}

void GameEventManager::ChestDrops(float chest_x, float chest_y) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0F, 1.0F);
    float const random_number = dis(gen);
    SDL_Log("Rand: %f", random_number);
    if (random_number <= m_ChanceOfDrop) {
        std::vector<GameObject*> items1;
        Properties props13("", {1, 1, 18, 16}, {chest_x, chest_y, 32, 32}, 0,
                           "chest1");
        auto* chest1 = new Chest(props13, ChestType::Wooden, items1, m_player);
        m_Objects.push_back(chest1);
        ColliderHandler::GetInstance()->AddCollider(chest1);
    }
}
