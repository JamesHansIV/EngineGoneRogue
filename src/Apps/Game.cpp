#include "Game.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Objects/Warrior.h"
#include "Engine/Events/EventListener.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Input/InputChecker.h"

Warrior* player = nullptr;
Warrior* player2 = nullptr;
Warrior* player3 = nullptr;
Warrior* player4 = nullptr;
Warrior* player5 = nullptr;
std::vector<Projectile*> projectiles;
std::vector<GameObject*> colliders;

Game::Game() {
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    Renderer::GetInstance()->AddTexture("player", "../assets/textures/Idle.png");
    Renderer::GetInstance()->AddTexture("tilemap", "../assets/textures/kenney_tiny-dungeon/Tilemap/tilemap_packed.png");
    Renderer::GetInstance()->AddTexture("player_run", "../assets/textures/Run.png");
    Renderer::GetInstance()->AddTexture("projectile", "../assets/textures/dot_PNG2.png");

    m_Map = new Map("tilemap");
    if (!m_Map->LoadMap("../assets/maps/tiny_dungeon1.txt")) {
        SDL_Log("Failed to load map\n");
        assert(false);
    }

    Properties props("player", {0, 0, 136, 96}, {0, 0, 136, 96});
    player = new Warrior(props);

    Properties props2("player",{0, 0, 136, 96}, {100, 128, 136, 96});
    player2 = new Warrior(props2);

    Properties props3("player", {0, 0, 136, 96}, {700, 360, 136, 96});
    Properties props4("player", {0, 0, 136, 96}, {300, 278, 136, 96});
    Properties props5("player", {0, 0, 136, 96}, {200, 389, 136, 96});
    player3 = new Warrior(props3);
    player4 = new Warrior(props4);
    player5 = new Warrior(props5);

    colliders.push_back(player2);
    colliders.push_back(player3);
    colliders.push_back(player4);
    colliders.push_back(player5);

    GetEventManager().addListener(*player);

    Renderer::GetInstance()->SetCameraTarget(player);

    LoadProject();
}

void Game::LoadObject(tinyxml2::XMLElement* xmlObj) {
    tinyxml2::XMLElement* textureID = xmlObj->FirstChildElement("TextureID");
    tinyxml2::XMLElement* objectID = xmlObj->FirstChildElement("ObjectID");
    tinyxml2::XMLElement* srcRect = xmlObj->FirstChildElement("SrcRect");
    tinyxml2::XMLElement* dstRect = xmlObj->FirstChildElement("DstRect");

    std::string textureIDVal = textureID->GetText();
    std::string objectIDVal = objectID->GetText();
    SDL_Log("Texture id: %s", textureIDVal.c_str());
    SDL_Log("Object id: %s", objectIDVal.c_str());
    TilePos tilePos;
    tilePos.row = atoi(srcRect->FirstChildElement("Row")->GetText());
    tilePos.col = atoi(srcRect->FirstChildElement("Column")->GetText());
    tilePos.w = atoi(srcRect->FirstChildElement("Width")->GetText());
    tilePos.h = atoi(srcRect->FirstChildElement("Height")->GetText());

    SDL_Log("Src row: %d", tilePos.row);
    SDL_Log("Src col: %d", tilePos.col);
    SDL_Log("Src w: %d", tilePos.w);
    SDL_Log("Src h: %d", tilePos.h);

    Rect dstRectVal;
    dstRectVal.x = std::stof(dstRect->FirstChildElement("XPos")->GetText());
    dstRectVal.y = std::stof(dstRect->FirstChildElement("YPos")->GetText());
    dstRectVal.w = atoi(dstRect->FirstChildElement("Width")->GetText());
    dstRectVal.h = atoi(dstRect->FirstChildElement("Height")->GetText());

    SDL_Log("Dst x: %f", dstRectVal.x);
    SDL_Log("Dst y: %f", dstRectVal.y);
    SDL_Log("dst w: %d", dstRectVal.w);
    SDL_Log("dst h: %d", dstRectVal.h);

    Properties props(
        textureIDVal, tilePos,
        dstRectVal, objectIDVal
    );

    m_Objects.push_back(new GameObject(props));
}

void Game::LoadProject() {
    tinyxml2::XMLDocument doc;
    char filePath[128];
    sprintf(filePath, "../assets/projects/%s.xml", m_ProjectName.c_str());
    tinyxml2::XMLError error = doc.LoadFile(filePath);
    assert(error == tinyxml2::XML_SUCCESS);

    tinyxml2::XMLElement* root = doc.FirstChildElement("Root");
    tinyxml2::XMLElement* currObject = root->FirstChildElement("Object");

    while (currObject != nullptr) {
        LoadObject(currObject);
        currObject = currObject->NextSiblingElement("Object");
    }
}

void Game::Update(float dt) {
    player->Update(dt,colliders);
    int playerX = player->GetMidPointX() - Renderer::GetInstance()->GetCameraX();
    int playerY = player->GetMidPointY() - Renderer::GetInstance()->GetCameraY();

    SDL_Log("%d, %d", playerX , playerY );
    int mouseX = InputChecker::getMouseX();
    int mouseY = InputChecker::getMouseY();

    SDL_Log("%d, %d", mouseX , mouseY );

    // Calculate the angle between the mouse and the player
    float deltaX = mouseX - playerX;
    float deltaY = mouseY - playerY;
    float angle = atan2(deltaY, deltaX) * (180.0 / M_PI);
    // Convert the angle range from -180 to 180 to 0 to 360
    if (angle < 0) {
        angle += 360.0f;
    }

    SDL_Log("%f", angle);

    Properties projectile_props("projectile", {0, 0, 723, 724}, {player->GetMidPointX(), player->GetMidPointY(), 15, 15});
    if (InputChecker::isMouseButtonPressed(SDL_BUTTON_LEFT))
    {
        Projectile* projectile = nullptr;
        projectile = new Projectile(projectile_props, 50, 1.0, angle);
        projectiles.push_back(projectile);
        InputChecker::setMouseButtonPressed(SDL_BUTTON_LEFT, false);
    }
    for (auto projectile : projectiles) {
        projectile->Update(dt);
    }
}

void Game::Render() {
    Renderer::GetInstance()->RenderClear();
    m_Map->Draw();
    player->Draw();
    player2->Draw();
    player3->Draw();
    player4->Draw();
    player5->Draw();
    for (auto projectile : projectiles) {
        projectile->Draw();
    }

    Renderer::GetInstance()->Render();
}

#if EDITOR == 0
Application* CreateApplication() {
    return new Game();
}
#endif
