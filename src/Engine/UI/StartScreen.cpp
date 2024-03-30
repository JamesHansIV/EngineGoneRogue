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
        Button("buttons", SDL_Rect{x, y, 100, 60}, {"Start"}, [](auto& button) {
            SDL_Log("Start button clicked");
            timer.Unpause();
            PushNewEvent(EventType::StartGameEvent);
        });
    int const endless_x = (window_width - 150) / 2;

    m_toggle_endless = Button("buttons", SDL_Rect{endless_x, y + 60, 150, 80},
                              {"Normal", "Endless"}, [](auto& button) {
                                  SDL_Log("Toggle endless button clicked");
                                  Game& game =
                                      static_cast<Game&>(Application::Get());
                                  if (!game.GetEndless()) {
                                      button.SetText("Normal");
                                      game.SetEndless(true);
                                  } else {
                                      button.SetText("Endless");
                                      game.SetEndless(false);
                                  }
                              });
}

StartScreen::~StartScreen() {}

void StartScreen::Draw() {
    m_background.Draw(m_background_src);
    m_start_button.Draw();
    m_toggle_endless.Draw();
}

void StartScreen::Update() {
    const int window_width = Application::Get().GetWindowWidth();
    const int window_height = Application::Get().GetWindowHeight();
    m_background.ChangeDst({0, 0, window_width, window_height});
    m_start_button.Update();
    m_toggle_endless.Update();
}

State* StartScreen::HandleEvent(Event* event) {
    if (event->GetEventType() == EventType::MouseButtonDownEvent) {
        //m_start_button.HandleEvent(static_cast<MouseButtonDownEvent*>(event));
    }
    return nullptr;
}
