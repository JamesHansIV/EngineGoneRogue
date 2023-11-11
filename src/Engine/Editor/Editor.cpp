#define IMGUI_IMPL_API

#include "Editor.h"
#include "imgui.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Player.h"
#include "Engine/Objects/Projectile.h"
#include "Engine/Input/InputChecker.h"

#include <cstdlib>
#include <dirent.h>
#include <memory>
#include <sys/stat.h>
#include <tinyxml2.h>

/*
TODO list
(x = finished)
(~ = in progress)
- show level/grid info
x Move/resize objects
x rotate objects
- change color of objects
x maintain a selected object
x maintain a selected texture
x Show a list of textures
x show a list of objects
- Add multiple textures to an object?
x Add snap to grid
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
x Create list of game objects and their required information
- main menu bar
- action management and undoing actions
x create/load/save rooms
- create/load/save projects
- add auto tiling
- add file browser
x change topmost tree nodes to tabs
- delete objects
*/

struct stat info;

const char* object_type_strs[] = {"Base", "Projectile", "Player"};

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

    SDL_Log("editor constructor");
    m_Rooms = Application::m_Rooms;
    m_Layers.emplace_back();
}

Editor::~Editor() {
        //TODO: check if cleanup is successful
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    CleanLayers();
}

void Editor::CleanLayers() {
    m_HiddenLayers.clear();
    m_CurrentTexture = nullptr;
    m_CurrentObject = nullptr;
    auto it = m_Layers.begin();
    for (int i = 0; i < m_Layers.size(); i++) {
        for (auto *obj : m_Layers[i]) {
            delete obj;
        }
        m_Layers.erase(it);
        it++;
    }
}

void SaveBaseObject(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj, GameObject* obj) {
    tinyxml2::XMLElement* texture_id = doc.NewElement("TextureID");
    tinyxml2::XMLElement* object_id = doc.NewElement("ObjectID");
    tinyxml2::XMLElement* src_rect = doc.NewElement("SrcRect");
    tinyxml2::XMLElement* dst_rect = doc.NewElement("DstRect");
    tinyxml2::XMLElement* rotation = doc.NewElement("Rotation");

    texture_id->SetText(obj->GetTextureID().c_str());
    object_id->SetText(obj->GetID().c_str());

    TilePos const tile_pos = obj->GetTilePos();
    tinyxml2::XMLElement* row = doc.NewElement("Row");
    tinyxml2::XMLElement* column = doc.NewElement("Column");
    tinyxml2::XMLElement* src_width = doc.NewElement("Width");
    tinyxml2::XMLElement* src_height = doc.NewElement("Height");

    row->SetText(std::to_string(tile_pos.row).c_str());
    column->SetText(std::to_string(tile_pos.col).c_str());
    src_width->SetText(std::to_string(tile_pos.w).c_str());
    src_height->SetText(std::to_string(tile_pos.h).c_str());

    src_rect->InsertEndChild(row);
    src_rect->InsertEndChild(column);
    src_rect->InsertEndChild(src_width);
    src_rect->InsertEndChild(src_height);

    Rect const rect = obj->GetDstRect();
    tinyxml2::XMLElement* x_pos = doc.NewElement("XPos");
    tinyxml2::XMLElement* y_pos = doc.NewElement("YPos");
    tinyxml2::XMLElement* dst_width = doc.NewElement("Width");
    tinyxml2::XMLElement* dst_height = doc.NewElement("Height");

    x_pos->SetText(std::to_string(rect.x).c_str());
    y_pos->SetText(std::to_string(rect.y).c_str());
    dst_width->SetText(std::to_string(rect.w).c_str());
    dst_height->SetText(std::to_string(rect.h).c_str());

    dst_rect->InsertEndChild(x_pos);
    dst_rect->InsertEndChild(y_pos);
    dst_rect->InsertEndChild(dst_width);
    dst_rect->InsertEndChild(dst_height);

    rotation->SetText(std::to_string(obj->GetRotation()).c_str());

    xmlObj->InsertEndChild(texture_id);
    xmlObj->InsertEndChild(object_id);
    xmlObj->InsertEndChild(src_rect);
    xmlObj->InsertEndChild(dst_rect);
    xmlObj->InsertEndChild(rotation);
}

void Editor::SaveRoom(const char* roomName) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("Root");
    doc.InsertFirstChild(root);

    tinyxml2::XMLElement* curr_xml_object;
    for (const auto& layer : m_Layers) {
        for (auto *obj : layer) {
            curr_xml_object = doc.NewElement("Object");
            switch(obj->GetObjectType()) {
                case ObjectType::Base:
                    SaveBaseObject(doc, curr_xml_object, obj);
                    break;
                case ObjectType::Projectile:
                    break;
                case ObjectType::Player:
                    break;
                default:
                    SDL_LogError(0, "Invalid object type");
                    assert(false);
                    break;
            }
            root->InsertEndChild(curr_xml_object);
        }
    }

    char file_path[128];
    sprintf(file_path, "../assets/projects/%s/rooms/%s.xml", m_ProjectName.c_str(), roomName);
    int const success = doc.SaveFile(file_path);
    SDL_Log("Saving room a success: %d", success);
}


void Editor::SaveProject() {
    CreateProjectFolder();
    Renderer::GetInstance()->SaveTextures();
    for (const auto& room : m_Rooms) {
        SaveRoom(room.first.c_str());
    }
}

void Editor::CreateProjectFolder() {
    struct dirent* entry;
    DIR* dp;

    char project_path[128];
    sprintf(project_path, "../assets/projects/%s", m_ProjectName.c_str());
    dp = opendir(project_path);
    if (dp == nullptr) {
        char command[128];
        sprintf(command, "mkdir %s", project_path);
        system(command);
    }
    closedir(dp);
}

void Editor::SetObjectInfo() {
    auto* tile_map = dynamic_cast<TileMap*>(m_CurrentTexture);
    if (tile_map != nullptr) {
        m_ObjectInfo.Tile = { 0, 0, tile_map->GetTileSize(), tile_map->GetTileSize() };
        m_ObjectInfo.DstRect = { 0, 0, TILE_SIZE, TILE_SIZE };
    } else {
        m_ObjectInfo.Tile = { 0, 0, m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight() };
        m_ObjectInfo.DstRect = { 0, 0, m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight() };
    }
}

void Editor::ShowTextureIDs() {
    std::vector<std::string> const& texture_i_ds = Renderer::GetInstance()->GetTextureIDs();
    if (texture_i_ds.empty()) {
        ImGui::Text("No loaded textures");
    } else {
        if (ImGui::TreeNode("Select texture")) {
            for (const auto& id : texture_i_ds) {
                if (ImGui::Button(id.c_str(), ImVec2(100, 30))) {
                    m_CurrentTexture = Renderer::GetInstance()->GetTexture(id);
                    SetObjectInfo();
                }
            }
            ImGui::TreePop();
        }
    }
}

void Editor::AddRoom() {
    std::vector<GameObject*> objects;
    for (const auto& row : m_Layers) {
        for (auto *obj: row) {
            objects.push_back(new GameObject(obj));
        }
    }
    m_Rooms[m_CurrentRoomID] = std::vector<GameObject*>(objects);
}

void Editor::ShowFileManager() {
    if (ImGui::BeginTabItem("File")) {
        ImGui::Text("Current room: %s", m_CurrentRoomID.c_str());

        if (ImGui::Button("Save Project", ImVec2(150, 20))) {
            SaveProject();
        }

        if (ImGui::Button("Save Room", ImVec2(150, 20))) {
            ImGui::OpenPopup("save_room");
}

        if (ImGui::BeginPopup("save_room")) {
            static char room_name[128];
            ImGui::InputText("Input room name", room_name, sizeof(room_name));
            if (strcmp(room_name, "") != 0) {
                if (ImGui::Button("Save", ImVec2(100, 30))) {
                    m_CurrentRoomID = room_name;
                    SaveRoom(m_CurrentRoomID.c_str());
                    AddRoom();
                }
            }
            ImGui::EndPopup();
        }

        if (ImGui::Button("Load Room", ImVec2(150, 20))) {
            ImGui::OpenPopup("load_room");
}

        if (ImGui::BeginPopup("load_room")) {
            for (const auto& item : m_Rooms) {
                std::string const id = item.first;
                SDL_Log("Room: %s", id.c_str());
                if (strcmp(id.c_str(), "") != 0) {
                    if (ImGui::Button(id.c_str(), ImVec2(100, 30))) {
                        if (!m_CurrentRoomID.empty()) {
                            SaveRoom(m_CurrentRoomID.c_str());
}
                            AddRoom();
                        CleanLayers();
                        m_Layers.push_back(m_Rooms[id]);
                        m_CurrentRoomID = id;
                    }

                }
            }
            ImGui::EndPopup();
        }
        ImGui::EndTabItem();
    }
}

void Editor::ShowObjectEditor() {
    if (ImGui::BeginTabItem("Object Editor")) {
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

        if (m_CurrentObject != nullptr) {
            ImGui::Text("Selected object: %s", m_CurrentObject->GetID().c_str());
            ImGui::Text("Texture:");
            Texture* obj_texture = Renderer::GetInstance()->GetTexture(m_CurrentObject->GetTextureID());

            ImVec2 size;
            ImVec2 uv0;
            ImVec2 uv1;

            if (auto* tile_map = dynamic_cast<TileMap*>(obj_texture)) {
                size = { static_cast<float>(tile_map->GetTileSize()) * 10, static_cast<float>(tile_map->GetTileSize()) * 10 };
                uv0 = ImVec2(
                    m_CurrentObject->GetTilePos().col / static_cast<float>(tile_map->GetCols()),
                    m_CurrentObject->GetTilePos().row / static_cast<float>(tile_map->GetRows())
                );
                uv1 = ImVec2(
                    (m_CurrentObject->GetTilePos().col + 1) / static_cast<float>(tile_map->GetCols()),
                    (m_CurrentObject->GetTilePos().row + 1) / static_cast<float>(tile_map->GetRows())
                );
            } else {
                size = ImVec2(obj_texture->GetWidth(), obj_texture->GetHeight());
                uv0 = { 0, 0 };
                uv1 = { 1, 1 };
            }
            
            ImGui::Image((void*) obj_texture->GetTexture(), size, uv0, uv1);

            ImGui::SliderFloat("X position", &m_CurrentObject->GetX(), 0, LEVEL_WIDTH - m_CurrentObject->GetWidth());
            ImGui::SliderFloat("Y position", &m_CurrentObject->GetY(), 0, LEVEL_HEIGHT - m_CurrentObject->GetHeight());

            ImGui::SliderInt("Width", &m_CurrentObject->GetWidth(), 0, LEVEL_WIDTH);
            ImGui::SliderInt("Height", &m_CurrentObject->GetHeight(), 0, LEVEL_HEIGHT);

            ImGui::Text("Snap to grid: ");
            ImGui::SameLine();
            const char* snap_to_grid = m_ObjectInfo.SnapToGrid ? "True" : "False";
            if (ImGui::Button(snap_to_grid, ImVec2(80, 30))) {
                m_ObjectInfo.SnapToGrid = !m_ObjectInfo.SnapToGrid;
            }

            if (ImGui::Button("Rotate left", ImVec2(100, 30))) {
                m_CurrentObject->GetRotation() -= 90.0F;
                m_CurrentObject->GetRotation() = static_cast<int>(m_CurrentObject->GetRotation()) % 360;
            }
            ImGui::SameLine();
            if (ImGui::Button("Rotate right", ImVec2(100, 30))) {
                m_CurrentObject->GetRotation() += 90.0F;
                m_CurrentObject->GetRotation() = static_cast<int>(m_CurrentObject->GetRotation()) % 360;
            }

            if (ImGui::Button("Delete object", ImVec2(100, 30))) {
                DeleteObject();
            }
        }

        ImGui::EndTabItem();
    }
}

void Editor::ShowLoadTexture() {
    if (ImGui::BeginTabItem("Load texture")) {
        ImGui::Text("Input a texture filepath");
        static char filepath[256] = "";
        static char texture_id[256] = "";
        ImGui::InputText("Filepath", filepath, sizeof(filepath));
        ImGui::InputText("Texture name", texture_id, sizeof(texture_id));

        static bool is_tile_map = false;
        ImGui::Checkbox("Tile map", &is_tile_map);
        static int tile_size = 0;
        static int rows = 0;
        static int cols = 0;
        if (is_tile_map) {
            ImGui::InputInt("Set tile size", &tile_size);
            ImGui::InputInt("Set rows", &rows);
            ImGui::InputInt("Set columns", &cols);
        }
        static char invalid_filepath[256] = "";
        if (strcmp(filepath, "") != 0 && strcmp(texture_id, "") != 0) {
            if (ImGui::Button("Load texture", ImVec2(100, 30))) {

                m_CurrentTexture = (is_tile_map) ?
                    Renderer::GetInstance()->AddTileMap(texture_id, filepath, tile_size, rows, cols) :
                    Renderer::GetInstance()->AddTexture(texture_id, filepath);
                
                if (m_CurrentTexture == nullptr) {
                    strcpy(invalid_filepath, filepath);
                } else {
                    SetObjectInfo();
                    strcpy(invalid_filepath, "");
                }
                strcpy(filepath, "");
                strcpy(texture_id, "");
                tile_size = 0;
                rows = 0;
                cols = 0;
                is_tile_map = false;
            }
        }

        if (strcmp(invalid_filepath, "") != 0) {
            ImGui::Text("ERROR: failed to load texture from %s", invalid_filepath);
}

        ImGui::EndTabItem();
    }
}

void Editor::AddObject(float x, float y) {
    GameObject* new_object;

    std::string obj_id = m_CurrentTexture->GetID();
    obj_id += std::to_string(m_CurrentTexture->GetObjectCount());

    m_ObjectInfo.DstRect.x = x;
    m_ObjectInfo.DstRect.y = y;

    Properties props(
        m_CurrentTexture->GetID(), m_ObjectInfo.Tile,
        m_ObjectInfo.DstRect, m_ObjectInfo.Rotation, obj_id
    );

    switch (m_ObjectInfo.type) {
        case ObjectType::Base:
            new_object = new GameObject(props);
            break;
        case ObjectType::Player:
            new_object = new Player(props);
            break;
        case ObjectType::Projectile:
            //Make this work with projectiles
            new_object = new GameObject(props);
            break;
        default:
            SDL_LogError(0, "Invalid object type");
            assert(false);
            break;
    }
    m_CurrentTexture->IncObjectCount();
    m_Layers[m_CurrentLayer].push_back(new_object);
}

void Editor::DeleteObject() {
    if (m_Rooms.find(m_CurrentRoomID) != m_Rooms.end()) {
        std::vector<GameObject*>& room = m_Rooms[m_CurrentRoomID];
        auto it = std::find(room.begin(), room.end(), m_CurrentObject);
        if (it != room.end()) {
            room.erase(it);
        }
    }
    for (auto& layer : m_Layers) {
        auto it = std::find(layer.begin(), layer.end(), m_CurrentObject);
        if (it != layer.end()) {
            SDL_Log("found obj in layer: %s", (*it)->GetID().c_str());
            layer.erase(it);
            SDL_Log("layer size: %lu", layer.size());
        }
    }
    SDL_Log("Successfully cleaned from layers");
    delete m_CurrentObject;
    m_CurrentObject = nullptr;
    SDL_Log("deleted object");
}

void Editor::ShowBuildPlayer() {
    ImGui::Text("Showing build player stuff");
}

void Editor::ShowBuildProjectile() {
    ImGui::Text("Showing build projectile stuff");
}

ObjectType Editor::ShowSelectObjectType() {
    static int current_index = 0; // Here we store our selection data as an index.
    const char* preview_value = object_type_strs[current_index];
    if (ImGui::BeginCombo("Select object type", preview_value, 0))
    {
        for (int i = 0; i < IM_ARRAYSIZE(object_type_strs); i++)
        {
            const bool is_selected = (current_index == i);
            if (ImGui::Selectable(object_type_strs[i], is_selected)) {
                current_index = i;
}

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
}
        }
        ImGui::EndCombo();
    }
    m_ObjectInfo.type = static_cast<ObjectType>(current_index+1);
    return static_cast<ObjectType>(current_index+1);
}

void Editor::ShowTiles(TileMap* tileMap) {
    ImGui::Text("Select tile");
    ImVec2 const size = ImVec2(tileMap->GetTileSize(), tileMap->GetTileSize());
    for (int i = 0; i < tileMap->GetRows(); i++) {
        for (int j = 0; j < tileMap->GetCols(); j++) {
            if (j != 0) {
                ImGui::SameLine();
}
            
            ImVec2 const uv0 = ImVec2(j / static_cast<float>(tileMap->GetCols()), i / static_cast<float>(tileMap->GetRows()));
            ImVec2 const uv1 = ImVec2(
                (j + 1) / static_cast<float>(tileMap->GetCols()),
                (i + 1) / static_cast<float>(tileMap->GetRows())
            );

            bool const is_active = tileMap->GetActiveButtons()[i][j];
            if (is_active) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(23, 30, 57, 255));
}
            
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(23, 30, 57, 255));

            if (ImGui::ImageButton("", (ImTextureID) tileMap->GetTexture(), size, uv0, uv1) || ImGui::IsItemClicked()) {
                tileMap->ClearButtons();
                m_ObjectInfo.Tile.row = i;
                m_ObjectInfo.Tile.col = j;
                tileMap->GetActiveButtons()[i][j] = !is_active;
            }

            ImGui::PopStyleColor();
            if (is_active) ImGui::PopStyleColor();
        }
    }
}

void Editor::ShowCreateObject() {
    if (ImGui::BeginTabItem("Create object")) {
        ShowTextureIDs();

        if (m_CurrentTexture == nullptr) {
            ImGui::Text("Please select a texture to create an object");
        } else {
            ImGui::Text("Selected texture: %s", m_CurrentTexture->GetID().c_str());
            ImGui::Image((void*) m_CurrentTexture->GetTexture(), ImVec2(m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight()));
            ImGui::Text("size = %d x %d", m_CurrentTexture->GetWidth(), m_CurrentTexture->GetHeight());

            auto* tile_map = dynamic_cast<TileMap*>(m_CurrentTexture);
            if (tile_map != nullptr) {
                // ImGui::SliderInt("Select tile row", &m_ObjectInfo.Tile.row, 0, tileMap->GetRows() - 1);
                // ImGui::SliderInt("Select tile column", &m_ObjectInfo.Tile.col, 0, tileMap->GetCols() - 1);
                ShowTiles(tile_map);
            }

            ImGui::SeparatorText("Select dimensions");
            ImGui::SliderInt("Select destination width: ", &m_ObjectInfo.DstRect.w, 0, LEVEL_WIDTH);
            ImGui::SliderInt("Select destination height: ", &m_ObjectInfo.DstRect.h, 0, LEVEL_HEIGHT);

            ObjectType const type = ShowSelectObjectType();
            switch(type) {
                case ObjectType::Base:
                    break;
                case ObjectType::Player:
                    ShowBuildPlayer();
                    break;
                case ObjectType::Projectile:
                    ShowBuildProjectile();
                    break;
                default:
                    SDL_LogError(0, "Invalid object type");
                    assert(false);
            }
            std::string const button_label = m_DrawState.DrawMode ? "Stop Draw" : "Draw";

            if (ImGui::Button(button_label.c_str())) {
                m_DrawState.DrawMode = !m_DrawState.DrawMode;
            }
        }
        ImGui::EndTabItem();
    }
}

void Editor::ShowChooseLayer() {
    if (ImGui::BeginTabItem("Layers")) {
        if (ImGui::Button("Add Layer", ImVec2(100, 30))) {
            m_Layers.emplace_back();
        }
        ImGui::SeparatorText("Select Layer");
        for (int i = 0; i < m_Layers.size(); i++) {
            char layer_label[32];
            sprintf(layer_label, "Layer %d", i);
            if (ImGui::Selectable(layer_label, m_CurrentLayer == i)) {
                m_CurrentLayer = i;
                m_CurrentObject = nullptr;
            }

            char button_label[32];
            if (m_HiddenLayers.find(i) == m_HiddenLayers.end()) {
                sprintf(button_label, "Hide %d", i);
                if (ImGui::Button(button_label, ImVec2(60, 20))) {
                    SDL_Log("layer %d should be hidden", i);
                    m_HiddenLayers.insert(i);
                }
            } else {
                sprintf(button_label, "Show %d", i);
                if (ImGui::Button(button_label, ImVec2(60, 20))) {
                    m_HiddenLayers.erase(i);
                }
            }
        }

        ImGui::EndTabItem();
    }
}

void Editor::ShowObjectManager() {
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_MenuBar;

    ImGui::Begin("Game Object Manager", nullptr, window_flags);

    if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None)) {
        ShowFileManager();
        ShowChooseLayer();
        ShowLoadTexture();
        ShowObjectEditor();
        ShowCreateObject();

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void Editor::Update(float  /*dt*/) {
    if (InputChecker::isKeyPressed(SDLK_UP)) {
        Renderer::GetInstance()->MoveCameraY(-10.0F);
}
    if (InputChecker::isKeyPressed(SDLK_DOWN)) {
        Renderer::GetInstance()->MoveCameraY(10.0F);
}
    if (InputChecker::isKeyPressed(SDLK_LEFT)) {
        Renderer::GetInstance()->MoveCameraX(-10.0F);
}
    if (InputChecker::isKeyPressed(SDLK_RIGHT)) {
        Renderer::GetInstance()->MoveCameraX(10.0F);
}

    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
    ShowObjectManager();
}

void Editor::Render() {
    ImGui::Render();
    SDL_Log("About to render");
    Renderer::GetInstance()->RenderClear();
    for (int i = 0; i < m_Layers.size(); i++) {
        if (m_HiddenLayers.find(i) == m_HiddenLayers.end()) {
            for (auto *obj : m_Layers[i]) {
                obj->Draw();
            }
        }
    }

    DrawGrid();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    Renderer::GetInstance()->Render();
}

void Editor::OnMouseClicked(SDL_Event&  /*event*/) {
    if (m_DrawState.DrawMode) {
        m_DrawState.IsDrawing = true;
        float const x = (InputChecker::getMouseX() / TILE_SIZE) * TILE_SIZE;
        float const y = (InputChecker::getMouseY() / TILE_SIZE) * TILE_SIZE;
        AddObject(x, y);
        m_DrawState.PrevX = x;
        m_DrawState.PrevY = y;

    } else {
        for (auto it = m_Layers[m_CurrentLayer].begin(); it != m_Layers[m_CurrentLayer].end(); it++) {
            if (CheckMouseOver(*it)) {
                m_CurrentObject = *it;
                m_Layers[m_CurrentLayer].erase(it);
                m_Layers[m_CurrentLayer].push_back(m_CurrentObject);
            }
        }
    }
}

void Editor::OnMouseMoved(SDL_Event& event) {
    if (InputChecker::isMouseButtonPressed(SDL_BUTTON_LEFT)) {

        if (m_DrawState.DrawMode && m_DrawState.IsDrawing) {
            float const x = (InputChecker::getMouseX() / TILE_SIZE) * TILE_SIZE;
            float const y = (InputChecker::getMouseY() / TILE_SIZE) * TILE_SIZE;

            if (x != m_DrawState.PrevX || y != m_DrawState.PrevY) {
                AddObject(x, y);
                m_DrawState.PrevX = x;
                m_DrawState.PrevY = y;
            }

        } else if (m_CurrentObject != nullptr && CheckMouseOver(m_CurrentObject)) {
            float const dx =  event.button.x - InputChecker::getMouseX();
            float const dy =  event.button.y - InputChecker::getMouseY();
            float const x = m_CurrentObject->GetX();
            float const y = m_CurrentObject->GetY();

            float next_x;
            float next_y;

            next_x = x + dx;
            next_y = y + dy;
            
            m_CurrentObject->SetX(next_x);
            m_CurrentObject->SetY(next_y);
        }
    }
}

void Editor::OnMouseUp(SDL_Event&  /*event*/) {
    if (InputChecker::isMouseButtonPressed(SDL_BUTTON_LEFT)) {
        if (m_DrawState.IsDrawing) {
            m_DrawState.IsDrawing = false;

        } else if (m_CurrentObject != nullptr && CheckMouseOver(m_CurrentObject)) {
        
            if (m_ObjectInfo.SnapToGrid) {
                std::pair<float, float> const coords = SnapToGrid(m_CurrentObject->GetX(), m_CurrentObject->GetY());
                m_CurrentObject->SetX(coords.first);
                m_CurrentObject->SetY(coords.second);
            }
        }
    }
}

std::pair<float, float> Editor::SnapToGrid(float x, float y) {
    float next_x;
    float next_y;
    next_x = (static_cast<int>(x + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;
    next_y = (static_cast<int>(y + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;

    return { next_x, next_y };
}

void Editor::Events() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        ImGuiIO const& io = ImGui::GetIO();
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
                OnMouseUp(event);
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
