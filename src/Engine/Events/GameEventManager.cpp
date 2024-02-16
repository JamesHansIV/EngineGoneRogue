#include "GameEventManager.h"
#include "Engine/Application/Application.h"
#include "Engine/Objects/Characters/Enemy.h"
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
                        m_player->HandleEvent(&death_event);
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
