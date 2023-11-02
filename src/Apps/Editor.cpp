#include "Editor.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Engine/Objects/GameObject.h"

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
}

Editor::~Editor() {
        //TODO: check if cleanup is successful
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

void Editor::ShowTilemap() {

    ImGui::Begin("SDL2/SDL_Renderer Texture Test");
    ImGui::Text("Input a texture or tilemap filepath");
    static char filepath[128] = "";
    static char textureID[128] = "";
    ImGui::InputText("Filepath", filepath, sizeof(filepath));
    ImGui::InputText("Texture name", textureID, sizeof(textureID));
    if (ImGui::Button("Load texture", ImVec2(100, 30))) {
        assert(filepath != "" && textureID != "");
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
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    ShowTilemap();
    // {
    //     static float f = 0.0f;
    //     static int counter = 0;

    //     ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    //     ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

    //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    //     if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
    //         counter++;
    //     ImGui::SameLine();
    //     ImGui::Text("counter = %d", counter);

    //     ImGui::End();
    // }
}


void Editor::Render() {
    ImGui::Render();
    Renderer::GetInstance()->RenderClear();
    
    for (auto& obj : m_Objects) {
        obj->Draw();
    }

    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    Renderer::GetInstance()->Render();
}


Application* CreateApplication() {
    return new Editor();
}