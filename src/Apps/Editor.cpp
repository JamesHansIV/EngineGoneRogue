#include "Editor.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Warrior.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/InputChecker.h"

#include <memory>

/*
TODO list
(x = finished)
- show level/grid info
x Move/resize objects
- rotate/change color of objects
x maintain a selected object
x maintain a selected texture
x Show a list of textures
x show a list of objects
- Add multiple textures to an object?
- Move objects at fine and coarse granularity
- Create object based on type
    - Add animations
    - Add physics info
    - add state info
    - Add abilities/stat/status info
- Create objects from tilemap
- Add layering
- Export objects to json file
    - information could also include
        - start position
        - room info
        - scene transition animations?
        - layer info
        - enemy spawn locations/times?
- Import objects into game class
- Create json object template
- Editor <-> game transitions
- Create list of game objects and their required information
- main menu bar
- action management and undoing actions
- create/load/save rooms
- create/load/save projects
*/

const char* OBJECT_TYPE_STRS[] = {"Base", "Projectile", "Warrior"};

Editor::Editor() : m_CurrentTexture(nullptr) {
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
    Texture* background = Renderer::GetInstance()->AddTexture("background", "../assets/textures/bg.png");
    assert(background != nullptr);
    Properties props("background", 0, 0, LEVEL_WIDTH, LEVEL_HEIGHT, LEVEL_WIDTH, LEVEL_HEIGHT, "background");
    m_Objects.push_back(new GameObject(props));
}

Editor::~Editor() {
        //TODO: check if cleanup is successful
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    for (auto obj : m_Objects) {
        delete obj;
    }
    delete m_Map;
}

void DrawGrid() {
    for (int i = 0; i < LEVEL_WIDTH; i += TILE_SIZE) {
        Renderer::GetInstance()->DrawLine(i, 0, i, LEVEL_HEIGHT);
    }
    for (int i = 0; i < LEVEL_HEIGHT; i += TILE_SIZE) {
        Renderer::GetInstance()->DrawLine(0, i, LEVEL_WIDTH, i);
    }
}

void Editor::ShowTextureIDs() {
    if (m_TextureIDs.size() == 0) {
        ImGui::Text("No loaded textures");
    } else {
        if (ImGui::TreeNode("Select texture")) {
            for (auto ID : m_TextureIDs) {
                if (ImGui::Button(ID.c_str(), ImVec2(100, 30))) {
                    m_CurrentTexture = Renderer::GetInstance()->GetTexture(ID);
                }
            }
            ImGui::TreePop();
        }
    }
}

void Editor::ShowObjectEditor() {
    if (ImGui::TreeNode("Object Editor")) {
        if (ImGui::TreeNode("Object list")) {
            for (auto obj : m_Objects) {
                if (ImGui::Button(obj->GetID().c_str(), ImVec2(100, 30))) {
                    m_CurrentObject = obj;
                }
            }
            ImGui::TreePop();
        }

        if (m_CurrentObject) {
            ImGui::Text("Selected object: %s", m_CurrentObject->GetID().c_str());
            ImGui::Text("Texture:");
            Texture* objTexture = Renderer::GetInstance()->GetTexture(m_CurrentObject->GetTextureID());
            ImGui::Image((void*) objTexture->GetTexture(), ImVec2(objTexture->GetWidth(), objTexture->GetHeight()));

            ImGui::SliderFloat("X position", &m_CurrentObject->GetX(), 0, LEVEL_WIDTH - m_CurrentObject->GetWidth());
            ImGui::SliderFloat("Y position", &m_CurrentObject->GetY(), 0, LEVEL_HEIGHT - m_CurrentObject->GetHeight());

            ImGui::SliderInt("Width", &m_CurrentObject->GetWidth(), 0, LEVEL_WIDTH);
            ImGui::SliderInt("Height", &m_CurrentObject->GetHeight(), 0, LEVEL_HEIGHT);
        }

        ImGui::TreePop();
    }

}

void Editor::ShowLoadTexture() {
    if (ImGui::TreeNode("Load texture")) {
        ImGui::Text("Input a texture or tilemap filepath");
        static char filepath[256] = "";
        static char textureID[256] = "";
        ImGui::InputText("Filepath", filepath, sizeof(filepath));
        ImGui::InputText("Texture name", textureID, sizeof(textureID));

        static char invalidFilepath[256] = "";
        if (ImGui::Button("Load texture", ImVec2(100, 30))) {
            assert(strcmp(filepath, "") != 0 && strcmp(textureID, "") != 0);
            SDL_Log("filepath: %s", filepath);
            m_CurrentTexture = Renderer::GetInstance()->AddTexture(textureID, filepath);
            if (!m_CurrentTexture) {
                strcpy(invalidFilepath, filepath);
            } else {
                strcpy(invalidFilepath, "");
                m_TextureIDs.push_back(textureID);
            }
            strcpy(filepath, "");
            strcpy(textureID, "");
        }
        if (strcmp(invalidFilepath, "") != 0)
            ImGui::Text("ERROR: failed to load texture from %s", invalidFilepath);
        ImGui::TreePop();
    }
}

void Editor::CreateObject(ObjectType type) {
    if (ImGui::Button("Add object", ImVec2(100, 30))) {
        std::string objID = m_CurrentTexture->GetID();
        objID += std::to_string(m_CurrentTexture->GetObjectCount());
        Properties props(
            m_CurrentTexture->GetID(), 0, 0,
            m_CurrentTexture->GetWidth(),
            m_CurrentTexture->GetHeight(),
            m_CurrentTexture->GetWidth(),
            m_CurrentTexture->GetHeight(),
            objID
        );
        m_CurrentTexture->IncObjectCount();
        switch(type) {
            case ObjectType::Base:
                m_Objects.push_back(new GameObject(props));
                break;
            case ObjectType::Warrior:
                m_Objects.push_back(new Warrior(props));
                break;
            case ObjectType::Projectile:
                m_Objects.push_back(new Projectile(props));
                break;
            default:
                SDL_LogError(0, "Invalid object type");
                assert(false);
        }
    }
}

void Editor::ShowCreatePlayer() {
    ImGui::Text("Showing create player stuff");
    CreateObject(ObjectType::Warrior);
}

void Editor::ShowCreateProjectile() {
    ImGui::Text("Showing create projectile stuff");
    CreateObject(ObjectType::Projectile);
}

void Editor::ShowSelectObjectType() {
    static int currentIndex = 0; // Here we store our selection data as an index.
    const char* previewValue = OBJECT_TYPE_STRS[currentIndex];
    if (ImGui::BeginCombo("Select object type", previewValue, 0))
    {
        for (int i = 0; i < IM_ARRAYSIZE(OBJECT_TYPE_STRS); i++)
        {
            const bool isSelected = (currentIndex == i);
            if (ImGui::Selectable(OBJECT_TYPE_STRS[i], isSelected))
                currentIndex = i;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    switch((ObjectType)(currentIndex+1)) {
        case ObjectType::Base:
            CreateObject(ObjectType::Base);
            break;
        case ObjectType::Warrior:
            ShowCreatePlayer();
            break;
        case ObjectType::Projectile:
            ShowCreateProjectile();
            break;
        default:
            SDL_LogError(0, "Invalid object type");
            assert(false);
    }
}

void Editor::ShowCreateObject() {
    if (ImGui::TreeNode("Create object")) {
        ShowTextureIDs();

        if (m_CurrentTexture == nullptr) {
            ImGui::Text("Please select a texture to create an object");
        } else {
            ImGui::Text("Selected texture: %s", m_CurrentTexture->GetID().c_str());
            ImGui::Image((void*) m_CurrentTexture->GetTexture(), ImVec2(m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight()));
            ImGui::Text("size = %d x %d", m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight());

            ShowSelectObjectType();
            
        }
        ImGui::TreePop();
    }
}

void Editor::ShowObjectManager() {
    ImGui::Begin("Game Object Manager");

    ShowLoadTexture();
    ShowObjectEditor();
    ShowCreateObject();
    
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

    ImGui::ShowDemoWindow();
    ShowObjectManager();
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