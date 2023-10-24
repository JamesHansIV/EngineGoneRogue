#include "Warrior.h"
#include "../Graphics/TextureManager.h"

Warrior::Warrior(Properties* props): Character(props){
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

    m_Transform->TranslateX(m_RigidBody->Position().X);
    m_Transform->TranslateY(m_RigidBody->Position().Y);

    m_Animation->Update();
}

void Warrior::OnEvent(Event& event) {
    SDL_Event& e = event.getEvent();
    
    if (e.type == SDL_KEYDOWN) {
        switch(e.key.keysym.sym )
        {
            case SDLK_UP:
            SDL_Log("Warrior move up\n");
            m_Transform->TranslateY(m_RigidBody->Position().Y - 50);
            break;

            case SDLK_DOWN:
            SDL_Log("Warrior move down\n");
            m_Transform->TranslateY(m_RigidBody->Position().Y + 50);
            break;

            case SDLK_LEFT:
            SDL_Log("Warrior move left\n");
            m_Transform->TranslateX(m_RigidBody->Position().X - 50);
            break;

            case SDLK_RIGHT:
            SDL_Log("Warrior move right\n");
            m_Transform->TranslateX(m_RigidBody->Position().X + 50);
            break;

            default:
            break;
        }
    }
}

void Warrior::Clean(){
    TextureManager::GetInstance()->Clean();
}

