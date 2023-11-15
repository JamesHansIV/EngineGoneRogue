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
std::vector<GameObject*> colliders;

Game::Game() {
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    Renderer::GetInstance()->AddTexture("player", "../assets/textures/spritesheets/player-front-idle.png");
    Renderer::GetInstance()->AddTexture("enemy1", "../assets/textures/spritesheets/enemy1_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("enemy2", "../assets/textures/spritesheets/enemy2_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("enemy3", "../assets/textures/spritesheets/enemy3_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("enemy4", "../assets/textures/spritesheets/enemy4_idle_spritesheet.png");
    Renderer::GetInstance()->AddTexture("enemy5", "../assets/textures/spritesheets/enemy5_idle_spritesheet.png");
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

    Properties props_p("player", {0, 0, 18, 16}, {0, 0, 18, 16});
    player = new Player(props_p);

    Properties props1("enemy1",{0, 0, 18, 16}, {200, 200, 18, 16});
    enemy1 = new Enemy(props1);

    Properties props2("enemy2", {0, 0, 18, 16}, {300, 260, 18, 16});
    enemy2 = new Enemy(props2);

    Properties props3("enemy3", {0, 0, 18, 16}, {500, 200, 18, 16});
    enemy3 = new Enemy(props3);
    
    Properties props4("enemy4", {0, 0, 18, 16}, {800, 267, 18, 16});
    enemy4 = new Enemy(props4);

    Properties props5("enemy5", {0, 0, 18, 16}, {700, 300, 18, 16});
    enemy5 = new Enemy(props5);
    
    Properties props6("enemy5", {0, 0, 18, 16}, {600, 150, 18, 16});
    enemy6 = new Enemy(props6);

    Properties props7("enemy5", {0, 0, 18, 16}, {700, 299, 18, 16});
    enemy7 = new Enemy(props7);

    m_Objects.push_back(enemy1);
    m_Objects.push_back(enemy2);
    m_Objects.push_back(enemy3);
    m_Objects.push_back(enemy4);
    m_Objects.push_back(enemy5);
    m_Objects.push_back(enemy6);
    m_Objects.push_back(enemy7);
    colliders.push_back(enemy1);
    colliders.push_back(enemy2);
    colliders.push_back(enemy3);
    colliders.push_back(enemy4);
    colliders.push_back(enemy5);
    colliders.push_back(enemy6);
    colliders.push_back(enemy7);

    GetEventManager().AddListener(*player);

    Renderer::GetInstance()->SetCameraTarget(player);
}

void Game::Update(float dt) {
    player->UpdateColliders(colliders);
    player->Update(dt);
    for (auto it = colliders.begin(); it != colliders.end();)
    {
        (*it)->Update(dt);
        Enemy* enemy = dynamic_cast<Enemy*>(*it);  // Cast to Enemy type
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
