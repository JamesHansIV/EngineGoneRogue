#include "Warrior.h"
#include "../Graphics/TextureManager.h"
#include "../Core/InputChecker.h"

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

    if (InputChecker::isKeyPressed(SDLK_UP)) {
        SDL_Log("Warrior move up\n");
        m_Transform->TranslateY(m_RigidBody->Position().Y - 50);
    }
    if (InputChecker::isKeyPressed(SDLK_DOWN)) {
        SDL_Log("Warrior move down\n");
        m_Transform->TranslateY(m_RigidBody->Position().Y + 50);
    }
    if (InputChecker::isKeyPressed(SDLK_LEFT)) {
        SDL_Log("Warrior move left\n");
        m_Transform->TranslateX(m_RigidBody->Position().X - 50);
    }
    if (InputChecker::isKeyPressed(SDLK_RIGHT)) {
        SDL_Log("Warrior move right\n");
        m_Transform->TranslateX(m_RigidBody->Position().X + 50);
    }

    m_Animation->Update();
}

void Warrior::OnEvent(Event& event) {
}

void Warrior::Clean(){
    TextureManager::GetInstance()->Clean();
}

