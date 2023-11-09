#define IMGUI_IMPL_API

#include "Editor.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Warrior.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Input/InputChecker.h"


#include <tinyxml2.h>
#include <memory>

/*
TODO list
(x = finished)
(~ = in progress)
- show level/grid info
x Move/resize objects
- rotate/change color of objects
x maintain a selected object
x maintain a selected texture
x Show a list of textures
x show a list of objects
- Add multiple textures to an object?
- Move objects at fine and coarse granularity
~ Create object based on type
    - Add animations
    - Add physics info
    - add state info
    - Add abilities/stat/status info
x Create objects from tilemap
x Add layering
x Export objects to json file
    - information could also include
        - start position
        - room info
        - scene transition animations?
        - layer info
        - enemy spawn locations/times?
x Import objects into game class
x Create json object template
- Editor <-> game transitions
- Create list of game objects and their required information
- main menu bar
- action management and undoing actions
- create/load/save rooms
- create/load/save projects
*/

const char* OBJECT_TYPE_STRS[] = {"Base", "Projectile", "Warrior"};

void DrawGrid() {
    for (int i = 0; i < LEVEL_WIDTH; i += TILE_SIZE) {
        Renderer::GetInstance()->DrawLine(i, 0, i, LEVEL_HEIGHT);
    }
    for (int i = 0; i < LEVEL_HEIGHT; i += TILE_SIZE) {
        Renderer::GetInstance()->DrawLine(0, i, LEVEL_WIDTH, i);
    }
}

bool CheckMouseOver(GameObject* obj) {
    return ((obj)->GetX() <= InputChecker::getMouseX() &&
            InputChecker::getMouseX() <= (obj)->GetX() + (obj)->GetWidth() &&
            (obj)->GetY() <= InputChecker::getMouseY() &&
            InputChecker::getMouseY() <= (obj)->GetY() + (obj)->GetHeight());
}

Editor::Editor() : m_CurrentTexture(nullptr), m_CurrentLayer(0) {
    ImGui::CreateContext();
    SDL_Renderer* renderer = Renderer::GetInstance()->GetRenderer();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplSDL2_InitForSDLRenderer(GetWindow(), renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    m_Layers.push_back(std::vector<GameObject*>());
}

Editor::~Editor() {
        //TODO: check if cleanup is successful
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    for (auto layer : m_Layers) {
        for (auto obj : layer) {
            delete obj;
        }
    }
    // delete m_Map;
}

void SaveBaseObject(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj, GameObject* obj) {
    tinyxml2::XMLElement* textureID = doc.NewElement("TextureID");
    tinyxml2::XMLElement* objectID = doc.NewElement("ObjectID");
    tinyxml2::XMLElement* srcRect = doc.NewElement("SrcRect");
    tinyxml2::XMLElement* dstRect = doc.NewElement("DstRect");

    textureID->SetText(obj->GetTextureID().c_str());
    objectID->SetText(obj->GetID().c_str());

    TilePos tilePos = obj->GetTilePos();
    tinyxml2::XMLElement* row = doc.NewElement("Row");
    tinyxml2::XMLElement* column = doc.NewElement("Column");
    tinyxml2::XMLElement* srcWidth = doc.NewElement("Width");
    tinyxml2::XMLElement* srcHeight = doc.NewElement("Height");

    row->SetText(std::to_string(tilePos.row).c_str());
    column->SetText(std::to_string(tilePos.col).c_str());
    srcWidth->SetText(std::to_string(tilePos.w).c_str());
    srcHeight->SetText(std::to_string(tilePos.h).c_str());

    srcRect->InsertEndChild(row);
    srcRect->InsertEndChild(column);
    srcRect->InsertEndChild(srcWidth);
    srcRect->InsertEndChild(srcHeight);

    Rect rect = obj->GetDstRect();
    tinyxml2::XMLElement* xPos = doc.NewElement("XPos");
    tinyxml2::XMLElement* yPos = doc.NewElement("YPos");
    tinyxml2::XMLElement* dstWidth = doc.NewElement("Width");
    tinyxml2::XMLElement* dstHeight = doc.NewElement("Height");

    xPos->SetText(std::to_string(rect.x).c_str());
    yPos->SetText(std::to_string(rect.y).c_str());
    dstWidth->SetText(std::to_string(rect.w).c_str());
    dstHeight->SetText(std::to_string(rect.h).c_str());

    dstRect->InsertEndChild(xPos);
    dstRect->InsertEndChild(yPos);
    dstRect->InsertEndChild(dstWidth);
    dstRect->InsertEndChild(dstHeight);

    xmlObj->InsertEndChild(textureID);
    xmlObj->InsertEndChild(objectID);
    xmlObj->InsertEndChild(srcRect);
    xmlObj->InsertEndChild(dstRect);
}

void Editor::SaveProject() {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("Root");
    doc.InsertFirstChild(root);

    tinyxml2::XMLElement* currXMLObject;
    for (auto layer : m_Layers) {
        for (auto obj : layer) {
            currXMLObject = doc.NewElement("Object");
            switch(obj->GetObjectType()) {
                case ObjectType::Base:
                    SaveBaseObject(doc, currXMLObject, obj);
                    break;
                case ObjectType::Projectile:
                    break;
                case ObjectType::Warrior:
                    break;
            }
            root->InsertEndChild(currXMLObject);
        }
    }

    char filePath[128];
    sprintf(filePath, "../assets/projects/%s.xml", m_ProjectName.c_str());
    int success = doc.SaveFile(filePath);
    SDL_Log("Saving file a success: %d", success);
}

void Editor::ShowSaveProject() {
    if (ImGui::BeginPopupModal("Save Room?", NULL, 0))
    {
        SDL_Log("Showing modal window");

        static bool dont_ask_me_next_time = false;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::Checkbox("Don't ask me next time", &dont_ask_me_next_time);
        ImGui::PopStyleVar();

        if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); SaveProject(); }
        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
        ImGui::EndPopup();
    }
}


void Editor::SetObjectInfo() {
    TileMap* tileMap = dynamic_cast<TileMap*>(m_CurrentTexture);
    if (tileMap != nullptr) {
        m_ObjectInfo.Tile = { 0, 0, tileMap->GetTileSize(), tileMap->GetTileSize() };
        m_ObjectInfo.DstRect = { 0, 0, tileMap->GetTileSize(), tileMap->GetTileSize() };
    } else {
        m_ObjectInfo.Tile = { 0, 0, m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight() };
        m_ObjectInfo.DstRect = { 0, 0, m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight() };
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
                    SetObjectInfo();
                }
            }
            ImGui::TreePop();
        }
    }
}

void Editor::ShowMenuBar() {
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save")) {
                SaveProject();
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void Editor::ShowObjectEditor() {
    if (ImGui::TreeNode("Object Editor")) {
        if (ImGui::TreeNode("Object list")) {

            for (auto it = m_Layers[m_CurrentLayer].begin(); it != m_Layers[m_CurrentLayer].end(); it++) {
                if (ImGui::Button((*it)->GetID().c_str(), ImVec2(100, 30))) {
                    m_CurrentObject = *it;
                    m_Layers[m_CurrentLayer].erase(it);
                    m_Layers[m_CurrentLayer].insert(m_Layers[m_CurrentLayer].end(), m_CurrentObject);
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
        ImGui::Text("Input a texture filepath");
        static char filepath[256] = "";
        static char textureID[256] = "";
        ImGui::InputText("Filepath", filepath, sizeof(filepath));
        ImGui::InputText("Texture name", textureID, sizeof(textureID));

        static bool isTileMap = false;
        ImGui::Checkbox("Tile map", &isTileMap);
        static int tileSize = 0;
        static int rows = 0;
        static int cols = 0;
        if (isTileMap) {
            ImGui::InputInt("Set tile size", &tileSize);
            ImGui::InputInt("Set rows", &rows);
            ImGui::InputInt("Set columns", &cols);
        }
        static char invalidFilepath[256] = "";
        if (strcmp(filepath, "") != 0 && strcmp(textureID, "") != 0) {
            if (ImGui::Button("Load texture", ImVec2(100, 30))) {

                m_CurrentTexture = (isTileMap) ?
                    Renderer::GetInstance()->AddTileMap(textureID, filepath, tileSize, rows, cols) :
                    Renderer::GetInstance()->AddTexture(textureID, filepath);

                SetObjectInfo();

                if (!m_CurrentTexture) {
                    strcpy(invalidFilepath, filepath);
                } else {
                    strcpy(invalidFilepath, "");
                    m_TextureIDs.push_back(textureID);
                }
                strcpy(filepath, "");
                strcpy(textureID, "");
                tileSize = 0;
                rows = 0;
                cols = 0;
                isTileMap = false;
            }
        }

        if (strcmp(invalidFilepath, "") != 0)
            ImGui::Text("ERROR: failed to load texture from %s", invalidFilepath);
        ImGui::TreePop();
    }
}

void Editor::ShowCreateBaseObject() {
    if (ImGui::Button("Add object", ImVec2(100, 30))) {
        std::string objID = m_CurrentTexture->GetID();
        objID += std::to_string(m_CurrentTexture->GetObjectCount());

        Properties props(
            m_CurrentTexture->GetID(), m_ObjectInfo.Tile,
            m_ObjectInfo.DstRect, objID
        );
        m_CurrentTexture->IncObjectCount();
        m_Layers[m_CurrentLayer].push_back(new GameObject(props));
    }
}

void Editor::ShowCreatePlayer() {
    ImGui::Text("Showing create player stuff");
    if (ImGui::Button("Add player", ImVec2(100, 30))) {
        std::string objID = m_CurrentTexture->GetID();
        objID += std::to_string(m_CurrentTexture->GetObjectCount());

        Properties props(
            m_CurrentTexture->GetID(), m_ObjectInfo.Tile,
            m_ObjectInfo.DstRect, objID
        );
        m_CurrentTexture->IncObjectCount();
        m_Layers[m_CurrentLayer].push_back(new Warrior(props));
    }
}

void Editor::ShowCreateProjectile() {
    ImGui::Text("Showing create projectile stuff");
    // if (ImGui::Button("Add projectile", ImVec2(100, 30))) {
    //     std::string objID = m_CurrentTexture->GetID();
    //     objID += std::to_string(m_CurrentTexture->GetObjectCount());
        // Properties props(
        //     m_CurrentTexture->GetID(), m_ObjectInfo.Tile,
        //     m_ObjectInfo.DstRect, objID
        // );
    //     m_CurrentTexture->IncObjectCount();
    //     m_Layers[m_CurrentLayer].push_back(new Projectile(props));
    // }
}

ObjectType Editor::ShowSelectObjectType() {
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
    return (ObjectType)(currentIndex+1);
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

            TileMap* tileMap = dynamic_cast<TileMap*>(m_CurrentTexture);
            if (tileMap != nullptr) {
                ImGui::SliderInt("Select tile row", &m_ObjectInfo.Tile.row, 0, tileMap->GetRows() - 1);
                ImGui::SliderInt("Select tile column", &m_ObjectInfo.Tile.col, 0, tileMap->GetCols() - 1);
            }

            ImGui::SeparatorText("Select dimensions");
            ImGui::SliderInt("Select destination width: ", &m_ObjectInfo.DstRect.w, 0, LEVEL_WIDTH);
            ImGui::SliderInt("Select destination height: ", &m_ObjectInfo.DstRect.h, 0, LEVEL_HEIGHT);

            ObjectType type = ShowSelectObjectType();
            switch(type) {
                case ObjectType::Base:
                    ShowCreateBaseObject();
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
        ImGui::TreePop();
    }
}

void Editor::ShowChooseLayer() {
    if (ImGui::TreeNode("Layers")) {
        if (ImGui::Button("Add Layer", ImVec2(100, 30))) {
            m_Layers.push_back(std::vector<GameObject*>());
        }
        ImGui::SeparatorText("Select Layer");
        for (int i = 0; i < m_Layers.size(); i++) {
            char layerLabel[32];
            sprintf(layerLabel, "Layer %d", i);
            if (ImGui::Selectable(layerLabel, m_CurrentLayer == i)) {
                m_CurrentLayer = i;
                m_CurrentObject = nullptr;
            }

            char buttonLabel[32];
            if (m_HiddenLayers.find(i) == m_HiddenLayers.end()) {
                sprintf(buttonLabel, "Hide %d", i);
                if (ImGui::Button(buttonLabel, ImVec2(60, 20))) {
                    SDL_Log("layer %d should be hidden", i);
                    m_HiddenLayers.insert(i);
                }
            } else {
                sprintf(buttonLabel, "Show %d", i);
                if (ImGui::Button(buttonLabel, ImVec2(60, 20))) {
                    m_HiddenLayers.erase(i);
                }
            }
        }

        ImGui::TreePop();
    }
}

void Editor::ShowObjectManager() {
    ImGuiWindowFlags windowFlags = 0;
    windowFlags |= ImGuiWindowFlags_MenuBar;

    ImGui::Begin("Game Object Manager", NULL, windowFlags);

    ShowMenuBar();
    ShowChooseLayer();
    ShowLoadTexture();
    ShowObjectEditor();
    ShowCreateObject();

    ImGui::End();
}

void Editor::Update(float dt) {
    if (InputChecker::isKeyPressed(SDLK_UP))
        Renderer::GetInstance()->MoveCameraY(-10.0f);
    if (InputChecker::isKeyPressed(SDLK_DOWN))
        Renderer::GetInstance()->MoveCameraY(10.0f);
    if (InputChecker::isKeyPressed(SDLK_LEFT))
        Renderer::GetInstance()->MoveCameraX(-10.0f);
    if (InputChecker::isKeyPressed(SDLK_RIGHT))
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

    for (int i = 0; i < m_Layers.size(); i++) {
        if (m_HiddenLayers.find(i) == m_HiddenLayers.end()) {
            for (auto& obj : m_Layers[i]) {
                obj->Draw();
            }
        }
    }

    DrawGrid();
    //m_Map->Draw();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    Renderer::GetInstance()->Render();
}



void Editor::OnMouseClicked(SDL_Event& event) {
    for (auto it = m_Layers[m_CurrentLayer].begin(); it != m_Layers[m_CurrentLayer].end(); it++) {
        if (CheckMouseOver(*it)) {
            m_CurrentObject = *it;
            m_Layers[m_CurrentLayer].erase(it);
            m_Layers[m_CurrentLayer].push_back(m_CurrentObject);
        }
    }
}

void Editor::OnMouseMoved(SDL_Event& event) {
    if (InputChecker::isMouseButtonPressed(SDL_BUTTON_LEFT) &&
        m_CurrentObject != nullptr && CheckMouseOver(m_CurrentObject)) {
        float dx =  event.button.x - InputChecker::getMouseX();
        float dy =  event.button.y - InputChecker::getMouseY();

        float x = m_CurrentObject->GetX();
        float y = m_CurrentObject->GetY();
        m_CurrentObject->SetX(x + dx);
        m_CurrentObject->SetY(y + dy);
    }
}

void Editor::Events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        ImGuiIO& io = ImGui::GetIO();
        if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
            return;
        }
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
            case SDL_MOUSEBUTTONDOWN:
                InputChecker::setMouseButtonPressed(event.button.button, true);
                OnMouseClicked(event);
                break;
            case SDL_MOUSEBUTTONUP:
                InputChecker::setMouseButtonPressed(event.button.button, false);
                break;
            case SDL_MOUSEMOTION:
                OnMouseMoved(event);
                InputChecker::updateMousePosition(event.motion.x, event.motion.y);
                break;
        }
    }
}

#if EDITOR == 1
Application* CreateApplication() {
    return new Editor();
}
#endif
