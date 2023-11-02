#include "Warrior.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/InputChecker.h"

Warrior::Warrior(Properties& props): Character(props){
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 6, 80, SDL_FLIP_HORIZONTAL);
    m_RigidBody = new RigidBody();
}

void Warrior::Draw(){
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);
}

void Warrior::Update(float dt){
    m_RigidBody->Update(0.3);
    m_RigidBody->ApplyForceX(0);
    m_Animation->SetProps("player", 1, 6, 100);

    if (InputChecker::isKeyPressed(SDLK_UP)) {
        m_Transform->TranslateY(m_RigidBody->Position().Y - 10);
    }
    if (InputChecker::isKeyPressed(SDLK_DOWN)) {
        m_Transform->TranslateY(m_RigidBody->Position().Y + 10);
    }
    if (InputChecker::isKeyPressed(SDLK_LEFT)) {
        m_Transform->TranslateX(m_RigidBody->Position().X - 10);
        m_Animation->SetProps("player_run", 1, 8, 100, SDL_FLIP_HORIZONTAL);
    }
    if (InputChecker::isKeyPressed(SDLK_RIGHT)) {
        m_Transform->TranslateX(m_RigidBody->Position().X + 10);
        m_Animation->SetProps("player_run", 1, 8, 100);
    }

    m_Animation->Update();
}

void Warrior::OnEvent(Event& event) {
    SDL_Event e = event.getEvent();
    SDL_Log("%d, %d", e.key.keysym.sym, SDL_KEYUP);
    if (e.type == SDL_KEYUP) {
        SDL_Log("slkdfjs");
        if (e.key.keysym.sym == SDLK_LEFT) {
            SDL_Log("Left left");
            m_Animation->SetProps("player", 1, 6, 100, SDL_FLIP_HORIZONTAL);
        }
        if (e.key.keysym.sym == SDLK_RIGHT) {
            SDL_Log("Right left");
            m_Animation->SetProps("player", 1, 6, 100);
        }
    }
}

void Warrior::Clean(){
    Renderer::GetInstance()->Clean();
}

