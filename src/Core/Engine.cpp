#include "Engine.h"
#include "../Graphics/TextureManager.h"

#include "../Objects/Warrior.h"
#include "../Objects/GameObject.h"
#include "InputChecker.h"

#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "../globals.h"
#include "../Components/DrawData.h"
#include "../Components/Player.h"

#include <unistd.h>
#include <fstream>
#include <sstream>

Engine* Engine::m_Instance = nullptr;

bool DrawMap() {
    std::vector<Component*>& drawData = Engine::GetInstance()->GetComponents(ComponentType::DrawData);

    if (drawData.empty()) {
        SDL_Log("Nothing to draw");
        return false;
    }

    for (auto data : drawData) {
        DrawData* d = (DrawData*)data;
        GameObject* obj = Engine::GetInstance()->GetObject(d->ObjectID);
        Position* pos = (Position*)obj->GetComponent(ComponentType::Position);
        Dimensions* dim = (Dimensions*)obj->GetComponent(ComponentType::Dimensions);
        SDL_Rect srcRect { pos->Col * dim->SrcWidth, pos->Row * dim->SrcHeight, dim->SrcWidth, dim->SrcHeight };
        SDL_Rect dstRect { pos->DstX, pos->DstY, dim->DstWidth, dim->DstHeight };
        TextureManager::GetInstance()->Draw(d->TextureID, srcRect, dstRect);
    }
    return true;
}

bool LoadPlayer(std::string textureID) {
    Engine* engine = Engine::GetInstance();
    int currID = engine->GetCurrentObjID();

    GameObject* obj = new GameObject(currID);
    Position* pos = new Position(currID, 0, 0, 0, 0);
    Dimensions* dim = new Dimensions(currID, 136, 96, 136, 96);
    DrawData* draw = new DrawData(currID, textureID);
    Player* player = new Player(currID, [](int objectID) {
        GameObject* playerObj = Engine::GetInstance()->GetObject(objectID);
        Position* p = (Position*)playerObj->GetComponent(ComponentType::Position);
        if (InputChecker::isKeyPressed(SDLK_UP)) {
            SDL_Log("Player move up\n");
            p->DstY -= 10;
        }
        if (InputChecker::isKeyPressed(SDLK_DOWN)) {
            SDL_Log("Player move down\n");
            p->DstY += 10;
        }
        if (InputChecker::isKeyPressed(SDLK_LEFT)) {
            SDL_Log("Player move left\n");
            p->DstX -= 10;
        }
        if (InputChecker::isKeyPressed(SDLK_RIGHT)) {
            SDL_Log("Player move right\n");
            p->DstX += 10;
        }
    });
    obj->AddComponent(pos);
    obj->AddComponent(dim);
    obj->AddComponent(draw);
    obj->AddComponent(player);
    engine->AddComponent(pos);
    engine->AddComponent(dim);
    engine->AddComponent(draw);
    engine->AddComponent(player);

    engine->AddObject(obj);
    return true;
}

bool UpdatePlayer() {
    std::vector<Component*>& playerComponents = Engine::GetInstance()->GetComponents(ComponentType::Player);
    for (auto playerComponent : playerComponents) {
        Player* player = (Player*)playerComponent;
        player->OnUpdate(player->ObjectID);
    }
}

bool LoadMap(std::string filename, std::string textureID) {
    std::ifstream file;
    file.open(filename);

    std::string width = "";
    std::string height = "";
    std::string srcTileSize = "";
    std::string destTileSize = "";

    std::getline(file, width);
    if (width == "") {
        SDL_Log("Tile map contains no width\n");
        return false;
    }
    std::getline(file, height);
    if (height == "") {
        SDL_Log("Tile map contains no height\n");
        return false;
    }
    std::getline(file, srcTileSize);
    if (srcTileSize == "") {
        SDL_Log("Tile map contains no source tile size\n");
        return false;
    }
    std::getline(file, destTileSize);
    if (destTileSize == "") {
        SDL_Log("Tile map contains no destination tile size\n");
        return false;
    }
    int Width = stoi(width);
    int Height = stoi(height);
    int SrcTileSize = stoi(srcTileSize);
    int DestTileSize = stoi(destTileSize);

    std::string row;
    std::string entry;
    int i = 0;
    int j = 0;


    while (std::getline(file, row)) {
        std::stringstream ss(row);

        SDL_Log("row %d", i);
        while (ss >> entry) {
            if (entry.size() != 2) {
                SDL_Log("Tile map entry has incorrect size");
                return false;
            }
            int tileRow = int(entry[0] - '0');
            int tileColumn = int(entry[1] - '0');

            Engine* engine = Engine::GetInstance();
            int currID = engine->GetCurrentObjID();
            //Properties props(textureID, j * DestTileSize, i * DestTileSize, DestTileSize, SrcTileSize);
            GameObject* obj = new GameObject(currID);
            Position* pos = new Position(currID, tileRow, tileColumn, j * DestTileSize, i * DestTileSize);
            Dimensions* dim = new Dimensions(currID, SrcTileSize, SrcTileSize, DestTileSize, DestTileSize);
            DrawData* draw = new DrawData(currID, textureID);
            obj->AddComponent(pos);
            obj->AddComponent(dim);
            obj->AddComponent(draw);
            engine->AddComponent(pos);
            engine->AddComponent(dim);
            engine->AddComponent(draw);

            engine->AddObject(obj);
            SDL_Log("%d: (%d, %d)", j, tileRow * SrcTileSize, tileColumn * SrcTileSize);
            j++;
        }
        j = 0;
        i++;
    }
    return true;
}


bool Engine::Init(){

    if(SDL_Init(SDL_INIT_VIDEO)!=0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)!= 0){
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    m_Window = SDL_CreateWindow("Soft Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(m_Window == nullptr){
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(m_Renderer == nullptr){
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }
    
    m_CurrObjectID = 0;

    //TODO: note that the cwd is <projectDir>/build instead of <projectDir>.
    //      Set a working directory path macro to use absolute file paths
    TextureManager::GetInstance()->AddTexture("player", "../assets/textures/Idle.png");
    TextureManager::GetInstance()->AddTexture("tilemap", "../assets/textures/kenney_tiny-dungeon/Tilemap/tilemap_packed.png");
    //m_Map = new Map("tilemap");
    if (!LoadMap("../assets/maps/tiny_dungeon1.txt", "tilemap")) {
        SDL_Log("Failed to load map\n");
        return false;
    }

    if (!LoadPlayer("player")) {
        SDL_Log("Failed to load player\n");
        return false;
    }
    
    //Properties props("player", 0, 0, 136, 96);
    //player = new Warrior(props);

    //AddObject(player);

    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(m_Window, m_Renderer);
    ImGui_ImplSDLRenderer2_Init(m_Renderer);

    //m_EventHandler.addListener(*player);
    return m_IsRunning = true;
}

void Engine::AddComponent(Component* component) {
    ComponentType type = component->GetComponentType();
    if (m_ComponentStore.find(type) == m_ComponentStore.end()) {
        m_ComponentStore[type];
    }
    m_ComponentStore[type].push_back(component);
}

std::vector<Component*>& Engine::GetComponents(ComponentType type) {
    assert(m_ComponentStore.find(type) != m_ComponentStore.end());
    return m_ComponentStore[type];
}

void Engine::Update(float dt){
    UpdatePlayer();

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
        static int counter = 0;
        std::vector<Component*>& playerComponents = Engine::GetInstance()->GetComponents(ComponentType::Player);
        for (auto playerComponent : playerComponents) {
            Player* player = (Player*)playerComponent;
            GameObject* playerObj = Engine::GetInstance()->GetObject(player->ObjectID);
            Dimensions* dim = (Dimensions*)playerObj->GetComponent(ComponentType::Dimensions);
            DrawData* drawData = (DrawData*)playerObj->GetComponent(ComponentType::DrawData);

            ImGui::Begin("Edit player");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderInt("Height", &dim->DstHeight, 0, 500);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::SliderInt("Width", &dim->DstWidth, 0, 500);            // Edit 1 float using a slider from 0.0f to 1.0f
            static int r, g, b;

            ImGui::SliderInt("Red", &r, 0, 255);
            ImGui::SliderInt("Green", &g, 0, 255);
            ImGui::SliderInt("Blue", &b, 0, 255);
            TextureManager::GetInstance()->SetTextureColor(drawData->TextureID, (Uint8)r, (Uint8)g, (Uint8)b);

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::End();
        }

        
    }

}

void Engine::Render(){
    ImGui::Render();
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);
    DrawMap();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    //m_Map->Draw();
    //player->Draw();
    
    SDL_RenderPresent(m_Renderer);
}

void Engine::Events(){
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                Quit();
                return;
            case SDL_KEYDOWN:
                InputChecker::setKeyPressed(event.key.keysym.sym, true);
                break;
            case SDL_KEYUP:
                InputChecker::setKeyPressed(event.key.keysym.sym, false);
                break;
        }
    }
    m_EventHandler.handleEvent(event);
}

bool Engine::Clean(){

    for (auto it = m_ObjectStore.begin(); it != m_ObjectStore.end(); ++it) {
        
    }
    for (auto it = m_ObjectStore.begin(); it != m_ObjectStore.end(); ++it) {
        
    }

    //TODO: check if cleanup is successful
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();


    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    return true;
}

void Engine::Quit(){
    m_IsRunning = false;
}
