#include "Warrior.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/InputChecker.h"

Warrior::Warrior(Properties& props): Character(props){
    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 6, 80);
    m_RigidBody = new RigidBody();
}

void Warrior::Draw(){
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_DstRect.w, m_DstRect.h);
}

void Warrior::Update(float dt){
    m_RigidBody->Update(dt);
    m_RigidBody->ApplyForceX(0);
    if (InputChecker::isKeyPressed(SDLK_w)) {
        m_Transform->TranslateY(m_RigidBody->Position().Y - 10 * dt);
    }
    if (InputChecker::isKeyPressed(SDLK_s)) {
        m_Transform->TranslateY(m_RigidBody->Position().Y + 10 * dt);
    }
    if (InputChecker::isKeyPressed(SDLK_a)) {
        m_Transform->TranslateX(m_RigidBody->Position().X - 10 * dt);
        m_Animation->SetProps("player_run", 1, 8, 100, SDL_FLIP_HORIZONTAL);
        setFlip(SDL_FLIP_HORIZONTAL);
    }
    if (InputChecker::isKeyPressed(SDLK_d)) {
        m_Transform->TranslateX(m_RigidBody->Position().X + 10 * dt);
        m_Animation->SetProps("player_run", 1, 8, 100);
        setFlip(SDL_FLIP_NONE);

    }
    m_Animation->Update();
}

void Warrior::OnEvent(Event& event) {
    SDL_Event e = event.getEvent();
    if (e.type == SDL_KEYUP) {
        if (e.key.keysym.sym == SDLK_a) {
            m_Animation->SetProps("player", 1, 6, 100, SDL_FLIP_HORIZONTAL);
            setFlip(SDL_FLIP_HORIZONTAL);
        }
        if (e.key.keysym.sym == SDLK_d) {
            m_Animation->SetProps("player", 1, 6, 100);
            setFlip(SDL_FLIP_NONE);
        }
    }
}

void Warrior::Clean(){
    Renderer::GetInstance()->Clean();
}

