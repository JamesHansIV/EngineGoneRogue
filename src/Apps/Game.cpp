#include "Game.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Objects/Player.h"
#include "Engine/Objects/Enemy.h"
#include "Engine/Events/EventListener.h"
#include "Engine/Input/InputChecker.h"

Player* player = nullptr;
Enemy* enemy1 = nullptr;
Enemy* enemy2 = nullptr;
Enemy* enemy3 = nullptr;
Enemy* enemy4 = nullptr;
Enemy* enemy5 = nullptr;
Enemy* enemy6 = nullptr;
Enemy* enemy7 = nullptr;
Enemy* enemy8 = nullptr;
std::vector<GameObject*> colliders;

Game::Game() {
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    Renderer::GetInstance()->AddTexture("player", "../assets/textures/spritesheets/player-front-idle.png");
    Renderer::GetInstance()->AddTexture("enemy1", "../assets/textures/spritesheets/enemy1_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("enemy2", "../assets/textures/spritesheets/enemy2_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("enemy3", "../assets/textures/spritesheets/enemy3_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("enemy4", "../assets/textures/spritesheets/enemy4_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("enemy5", "../assets/textures/spritesheets/enemy5_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("boss", "../assets/textures/spritesheets/boss_idle_spritesheet.png" );
    Renderer::GetInstance()->AddTexture("tilemap", "../assets/textures/kenney_tiny-dungeon/Tilemap/tilemap_packed.png");
    Renderer::GetInstance()->AddTexture("player_run", "../assets/textures/Run.png");
    Renderer::GetInstance()->AddTexture("player_dead", "../assets/textures/spritesheets/player-dead.png");
    Renderer::GetInstance()->AddTexture("projectile", "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Firearms/Bullets/spr_bullet3.png");
    Renderer::GetInstance()->AddTexture("melee", "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Melee/spr_sword_03.png");
    Renderer::GetInstance()->AddTexture("gun", "../assets/textures/BulletHell/PURPLE/Weapons/weapons/Firearms/spr_weapon06.png");

    m_Objects = Application::m_Rooms["room1"];

    for (auto& obj : m_Objects) {
        if (obj->GetCollider() != nullptr) {
            colliders.push_back(obj);
        }
    }

    Properties props_p("player", {0, 0, 18, 18}, {0, 0, 36, 36});
    player = new Player(props_p);

    Properties props1("enemy5",{0, 0, 16, 16}, {200, 200, 36, 36});
    enemy1 = new Enemy(props1, 150, 150);

    Properties props2("enemy5", {0, 0, 16, 16}, {300, 260, 36, 36});
    enemy2 = new Enemy(props2,  150, 150);

    Properties props3("enemy5", {0, 0, 16, 16}, {500, 200, 36, 36});
    enemy3 = new Enemy(props3,  150, 150);
    
    Properties props4("enemy5", {0, 0, 16, 16}, {600, 367, 36, 36});
    enemy4 = new Enemy(props4,  150, 150);

    Properties props5("enemy5", {0, 0, 16, 16}, {700, 300, 36, 36});
    enemy5 = new Enemy(props5, 150, 150);
    
    Properties props6("enemy5", {0, 0, 16, 16}, {600, 150, 36, 36});
    enemy6 = new Enemy(props6,  150, 150);

    m_Objects.push_back(enemy1);
    m_Objects.push_back(enemy2);
    m_Objects.push_back(enemy3);
    m_Objects.push_back(enemy4);
    m_Objects.push_back(enemy5);
    m_Objects.push_back(enemy6);
    colliders.push_back(enemy1);
    colliders.push_back(enemy2);
    colliders.push_back(enemy3);
    colliders.push_back(enemy4);
    colliders.push_back(enemy5);
    colliders.push_back(enemy6);

    GetEventManager().AddListener(*player);

    Renderer::GetInstance()->SetCameraTarget(player);
}

void Game::Update(float dt) {
    if(player->IsMarkedForDeletion())
    {
        player->Clean();
        delete player;
    }
    player->UpdateColliders(colliders);
    player->Update(dt);
    for (auto it = colliders.begin(); it != colliders.end();)
    {
        Enemy* enemy = dynamic_cast<Enemy*>(*it);  // Cast to Enemy type
        if (enemy)
        {
            enemy->SetPlayer(player);
            enemy->UpdateColliders(colliders);
        }
        (*it)->Update(dt);
        if (enemy && enemy->IsMarkedForDeletion())  // Check if it's an Enemy and marked for deletion
        {
            DeleteObject(enemy);
            it = colliders.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void Game::Render() {
    Renderer::GetInstance()->RenderClear();
    // m_Map->Draw();
    for (auto *obj : m_Objects) {
        obj->Draw();
    }
    player->Draw();
    Renderer::GetInstance()->Render();
}

void Game::DeleteObject(GameObject* obj) {
    auto it = std::find(m_Objects.begin(), m_Objects.end(), obj);
    if (it != m_Objects.end()) {
        SDL_Log("found obj in m_Objects: %s", (*it)->GetID().c_str());
        m_Objects.erase(it);
        SDL_Log("m_Objects size: %lu", m_Objects.size());
    }
    obj->Clean();
    delete obj;
    obj = nullptr;
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif
