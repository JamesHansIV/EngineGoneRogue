#include "GameEventManager.h"
#include <random>
#include <utility>
#include <vector>
#include "Apps/Game.h"
#include "Engine/Application/Application.h"
#include "Engine/Events/Event.h"
#include "Engine/Objects/Characters/Enemy.h"
#include "Engine/Objects/Chests/Chest.h"
#include "Engine/Objects/Item.h"
#include "Engine/State/GameState.h"
#include "Engine/State/State.h"
#include "Engine/Timer/Timer.h"
#include "Engine/utils/utils.h"

CustomEventType custom_event_type = SDL_RegisterEvents(1);

GameEventManager::GameEventManager(Player* player,
                                   std::vector<GameObject*>& objects)
    : m_player(player), m_Objects(objects) {}

State* GameEventManager::HandleEvents(ItemManager* ItemManager,
                                      State* GameState) {
    SDL_Event event;
    UserEvent event_wrapper;
    event_wrapper.SetSDLEvent(&event);
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                Application::Get()->Quit();
                return nullptr;
            case SDL_KEYDOWN:
                InputChecker::SetKeyPressed(event.key.keysym.sym, true);
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        if (timer.IsPaused()) {
                            timer.Unpause();
                            return new RunningState(
                                static_cast<Game*>(Application::Get()));
                        } else {
                            timer.Pause();
                            return new PauseState(
                                static_cast<Game*>(Application::Get()));
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
            case SDL_MOUSEBUTTONDOWN: {
                InputChecker::SetMouseButtonPressed(event.button.button, true);
                const int x = event.button.x;
                const int y = event.button.y;
                MouseDownEvent e(x, y, event.button.button);
                State* state = GameState->HandleEvent(&e);
                if (state != nullptr) {
                    return state;
                }
                break;
            }
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
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    timer.Pause();
                    // Todo: Only pause after start state
                    //return new PauseState(
                    //    static_cast<Game*>(Application::Get()));
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
                            PlaceChestIfNeededEvent place_chest_event(
                                enemy->GetX(), enemy->GetY());
                            ItemManager->HandleEvent(&place_chest_event);
                        }
                        return nullptr;
                    }
                    case EventType::ChestOpenedEvent: {
                        auto* item = static_cast<std::vector<ItemType>*>(
                            event.user.data1);
                        auto* index = static_cast<std::pair<float, float>*>(
                            event.user.data2);
                        ChestOpenedEvent chest_open_event(*item, *index);
                        ItemManager->HandleEvent(&chest_open_event);
                        return nullptr;
                    }
                    case EventType::StartGameEvent: {
                        StartGameEvent start_game_event;
                        State* state =
                            GameState->HandleEvent(&start_game_event);
                        return state;
                    }
                    case EventType::ContinueGameEvent: {
                        ContinueGameEvent continue_game_event;
                        State* state =
                            GameState->HandleEvent(&continue_game_event);
                        return state;
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
            return nullptr;
        }
        if (m_player != nullptr) {
            m_player->HandleEvent(&event_wrapper);
        }
    }
    return nullptr;
}

// void GameEventManager::PlaceChestIfNeeded(float chest_x, float chest_y) {
//     std::random_device rd;
//     std::mt19937 gen(rd());
//     std::uniform_real_distribution<float> dis(0.0F, 1.0F);
//     float const random_number = dis(gen);
//     SDL_Log("Rand: %f", random_number);
//     if (random_number <= m_ChanceOfDrop) {
//         std::vector<ItemType> items1 = {Item1, Item2, Item3};
//         Properties props13("", {1, 1, 18, 16}, {chest_x, chest_y, 32, 32}, 0,
//                            "chest1");
//         auto* chest1 = new Chest(props13, ChestType::Wooden, items1, m_player);
//         m_Objects.push_back(chest1);
//         ColliderHandler::GetInstance()->AddCollider(chest1);
//     }
// }
