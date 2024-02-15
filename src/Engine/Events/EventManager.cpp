#include "EventManager.h"
#include "Engine/Application/Application.h"
#include "Engine/Timer/Timer.h"

void EventManager::HandleEvents() {
    SDL_Event event;
    SDL_Log("Event");
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            SDL_Log("Event type: %d", event.type);
            case SDL_QUIT:
                Application::Get()->Quit();
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
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    timer.Unpause();
                }
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    timer.Pause();
                }
                break;
        }
    }
}
