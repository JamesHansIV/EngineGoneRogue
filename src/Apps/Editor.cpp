#include "Editor.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

Editor::Editor() {
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

void Editor::Update(float dt) {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::End();
    }
}


void Editor::Render() {
    ImGui::Render();
    Renderer::GetInstance()->RenderClear();
    m_Map->Draw();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    Renderer::GetInstance()->Render();
}

