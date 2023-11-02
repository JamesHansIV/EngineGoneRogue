#include "Editor.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Engine/Objects/GameObject.h"
#include "Engine/InputChecker.h"

#include <memory>

Editor::Editor() : m_TextureID(""), m_CurrentTexture(nullptr) {
    ImGui::CreateContext();
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();
    ImGui_ImplSDL2_InitForSDLRenderer(GetWindow(), renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    Renderer::GetInstance()->AddTexture("tilemap", "../assets/textures/kenney_tiny-dungeon/Tilemap/tilemap_packed.png");
    m_Map = new Map("tilemap");
    if (!m_Map->LoadMap("../assets/maps/tiny_dungeon1.txt")) {
        SDL_Log("Failed to load map\n");
        assert(false);
    }
    Renderer::GetInstance()->AddTexture("background", "../assets/textures/bg.png");
    Properties props("background", 0, 0, LEVEL_WIDTH, LEVEL_HEIGHT);
    m_Objects.push_back(std::unique_ptr<GameObject>(new GameObject(props)));
}

Editor::~Editor() {
        //TODO: check if cleanup is successful
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void DrawGrid() {
    for (int i = 0; i < LEVEL_WIDTH; i += TILE_SIZE) {
        Renderer::GetInstance()->DrawLine(i, 0, i, LEVEL_HEIGHT);
    }
    for (int i = 0; i < LEVEL_HEIGHT; i += TILE_SIZE) {
        Renderer::GetInstance()->DrawLine(0, i, LEVEL_WIDTH, i);
    }
}

void Editor::ShowTilemap() {

    ImGui::Begin("SDL2/SDL_Renderer Texture Test");
    ImGui::Text("Input a texture or tilemap filepath");
    static char filepath[128] = "";
    static char textureID[128] = "";
    ImGui::InputText("Filepath", filepath, sizeof(filepath));
    ImGui::InputText("Texture name", textureID, sizeof(textureID));
    if (ImGui::Button("Load texture", ImVec2(100, 30))) {
        assert(strcmp(filepath, "") != 0 && strcmp(textureID, "") != 0);
        SDL_Log("filepath: %s", filepath);
        m_TextureID = textureID;
        m_CurrentTexture = Renderer::GetInstance()->AddTexture(m_TextureID, filepath);
        m_TextureIDs.insert(m_TextureID);
    }
    if (m_CurrentTexture != nullptr && m_TextureID != "") {
        ImGui::Text("size = %d x %d", m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight());
        ImGui::Image((void*) m_CurrentTexture->GetTexture(), ImVec2(m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight()));
        
        if (ImGui::Button("Add object", ImVec2(100, 30))) {
            Properties props(m_TextureID, 0, 0, m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight());
            m_Objects.push_back(std::unique_ptr<GameObject>(new GameObject(props)));
        }
    }
    ImGui::End();
}

void Editor::Update(float dt) {
    if (InputChecker::isKeyPressed(SDLK_w))
        Renderer::GetInstance()->MoveCameraY(-10.0f);
    if (InputChecker::isKeyPressed(SDLK_s))
        Renderer::GetInstance()->MoveCameraY(10.0f);
    if (InputChecker::isKeyPressed(SDLK_a))
        Renderer::GetInstance()->MoveCameraX(-10.0f);
    if (InputChecker::isKeyPressed(SDLK_d))
        Renderer::GetInstance()->MoveCameraX(10.0f);
    
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ShowTilemap();
}

void Editor::Render() {
    ImGui::Render();
    Renderer::GetInstance()->RenderClear();
    

    for (auto& obj : m_Objects) {
        obj->Draw();
    }
    DrawGrid();
    //m_Map->Draw();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    Renderer::GetInstance()->Render();
}

void Editor::Events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        GetEventManager().handleEvent(event);
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
}

#if EDITOR == 1
Application* CreateApplication() {
    return new Editor();
}
#endif