#include "StartScreen.h"
#include "Apps/Game.h"
#include "Engine/Objects/Characters/Player.h"

StartScreen::StartScreen() {
    const int window_width = Application::Get().GetWindowWidth();
    const int window_height = Application::Get().GetWindowHeight();

    Texture* texture =
        Renderer::GetInstance().GetTexture(kDefaultStartTextureID);

    m_background_src = {0, 0, texture->GetWidth(), texture->GetHeight()};

    m_background = DrawElement(kDefaultStartTextureID,
                               {0, 0, window_width, window_height});

    int const x = (window_width - 100) / 2;
    int const y = window_height - 170;
    m_start_button =
        Button("buttons", SDL_Rect{x, y, 100, 60}, {"Start"}, []() {
            timer.Unpause();
            PushNewEvent(EventType::StartGameEvent);
        });
    int const endless_x = (window_width - 150) / 2;

    m_normal_mode =
        Button("buttons", SDL_Rect{endless_x + 100, y + 60, 150, 80},
               {"Normal"}, []() {
                   Game& game = static_cast<Game&>(Application::Get());
                   game.SetEndless(false);
               });

    m_endless_mode =
        Button("buttons", SDL_Rect{endless_x - 100, y + 60, 150, 80},
               {"Endless"}, []() {
                   Game& game = static_cast<Game&>(Application::Get());

                   game.SetEndless(true);
               });
}

StartScreen::~StartScreen() = default;

void StartScreen::Draw() {
    m_background.Draw(m_background_src);
    m_start_button.Draw();
    m_normal_mode.Draw();
    m_endless_mode.Draw();
}

void StartScreen::Update() {
    Game& game = static_cast<Game&>(Application::Get());
    const int window_width = Application::Get().GetWindowWidth();
    const int window_height = Application::Get().GetWindowHeight();
    m_background.ChangeDst({0, 0, window_width, window_height});
    m_start_button.Update();
    m_normal_mode.Update();
    m_endless_mode.Update();
    if (game.GetEndless()) {
        m_endless_mode.SetState(ButtonState::ButtonStateHover);
        m_normal_mode.SetState(ButtonState::ButtonStateNormal);
    } else {
        m_endless_mode.SetState(ButtonState::ButtonStateNormal);
        m_normal_mode.SetState(ButtonState::ButtonStateHover);
    }
}

State* StartScreen::HandleEvent(Event* event) {
    if (event->GetEventType() == EventType::MouseButtonDownEvent) {
        //m_start_button.HandleEvent(static_cast<MouseButtonDownEvent*>(event));
    }
    return nullptr;
}
