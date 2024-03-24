#define IMGUI_IMPL_API

#include "Editor.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "imgui.h"

#include "Engine/Config/Config.h"
#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"

#include <dirent.h>
#include <sys/stat.h>
#include <cstdlib>
#include <memory>

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
x delete objects
- Add tool box containing all the editor tools


// Just spitting ideas, don't have to do all of this
- screen shake
- lighting
    - dark levels
    - torches
- audio
    - add music
    - add audio to weapons, enemies, player, projectiles

- environment
    - traps
    - trap doors
    - falling
    - chests and keys
- weapons
    - uzi
    - shotgun
    - sniper rifle
    - sword
    - pistol
    - charge shot
    - flamethrower
    - plasma rifle
    - ice ray
    - bow and arrow
    - laser beam weapon
    - black hole gun
    - rpg
    - grenade launcher
    - grenades
    - molotov
    - lightning weapon?
    - throwable knives/shurikens

- pickups
    - health
    - ammo
    - explosives
- shop items
    - items/modifiers
        - homing bullets
        - ricochet
        - speed
        - spawn allies/friendlies
        - max health upgrades
        - bullet size
        - damage
        - air strike
        - ammo supply drops
        - dodge roll bombs
        - decrease bullet spread
        - snake's cardboard box
        - double the amount of bullets
        - damage resistance
        - map foresight
    - grenades
    - weapons
    - keys
    - armor

- animations
    - firing animations
    - melee animations
    - better bullet sprites
    - 2nd character
    - explosions
    - dodge
    - reload
    - bullet collision

- enemies
    - new enemies
        - creatures e.g. dogs/insects
        - snipers
        - shield enemies
        - pistol enemies
        - rifle enemies
        - shotgun enemies
        - bullet hell spread
        - kamikaze
    - item drops
    - ai

- game ui
    - health
    - ammo
    - minimap
    - grenades
    - current weapon
    - character select screen
    - loading screen
    - game over screen
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
    return ((obj)->GetX() <= InputChecker::GetMouseX() +
                                 Renderer::GetInstance()->GetCameraX() &&
            InputChecker::GetMouseX() + Renderer::GetInstance()->GetCameraX() <=
                (obj)->GetX() + (obj)->GetWidth() &&
            (obj)->GetY() <= InputChecker::GetMouseY() +
                                 Renderer::GetInstance()->GetCameraY() &&
            InputChecker::GetMouseY() + Renderer::GetInstance()->GetCameraY() <=
                (obj)->GetY() + (obj)->GetHeight());
}

void MoveObject(GameObject* obj, int dx, int dy) {
    float const x = obj->GetX();
    float const y = obj->GetY();

    float next_x;
    float next_y;

    next_x = x + dx;
    next_y = y + dy;

    obj->SetX(next_x);
    obj->SetY(next_y);
}

std::vector<GameObject*> CopyObjects(const std::vector<GameObject*>& objects) {
    std::vector<GameObject*> object_copies;

    for (auto* obj : objects) {
        GameObject* new_obj = nullptr;
        if (auto* collider = dynamic_cast<Collider*>(obj)) {
            new_obj = new Collider(obj);
        } else {
            new_obj = new GameObject(obj);
        }
        object_copies.push_back(new_obj);
    }
    return object_copies;
}

Editor::Editor() {

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

    // load editor textures
    LoadEditorTextures();

    // create keymap
    m_KeyMap = new KeyMap();

    // create cursor
    m_Cursor = new Cursor();
}

Editor::~Editor() {
    //TODO: check if cleanup is successful
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    CleanLayers();

    delete m_KeyMap;
    delete m_Cursor;
}

void Editor::CleanLayers() {
    m_HiddenLayers.clear();
    m_CurrentTexture = nullptr;
    m_CurrentObject = nullptr;
    for (auto& m_layer : m_Layers) {
        for (auto* obj : m_layer) {
            delete obj;
        }
    }
    m_Layers.clear();
}

void Editor::SaveRoom(const char* roomName) {
    std::vector<GameObject*> objects;

    for (const auto& layer : m_Layers) {
        for (auto* obj : layer) {
            objects.push_back(obj);
        }
    }

    char file_path[FILEPATH_LEN + 1];
    snprintf(file_path, FILEPATH_LEN, "../assets/projects/%s/rooms/%s.xml",
             m_ProjectName.c_str(), roomName);

    int const success = SaveObjects(file_path, objects);
    SDL_Log("Saving room a success: %d", success);
}

void Editor::SaveProject() {
    CreateProjectFolder();

    char dst_path[FILEPATH_LEN + 1];
    snprintf(dst_path, FILEPATH_LEN, "../assets/projects/%s/textures.xml",
             GetProjectName().c_str());
    Renderer::GetInstance()->SaveTextures(dst_path);

    for (const auto& room : m_Rooms) {
        SaveRoom(room.first.c_str());
    }
}

void Editor::CreateProjectFolder() {
    struct dirent* entry;
    DIR* dp;

    char project_path[FILEPATH_LEN + 1];
    snprintf(project_path, FILEPATH_LEN, "../assets/projects/%s",
             m_ProjectName.c_str());
    dp = opendir(project_path);
    if (dp == nullptr) {
        char command[FILEPATH_LEN + 1];
        snprintf(command, FILEPATH_LEN, "mkdir %s", project_path);
        system(command);
    }
    closedir(dp);
}

void Editor::SetObjectInfo() {
    auto* tile_map = dynamic_cast<TileMap*>(m_CurrentTexture);
    if (tile_map != nullptr) {
        m_ObjectInfo.Tile = {0, 0, tile_map->GetTileSize(),
                             tile_map->GetTileSize()};
        m_ObjectInfo.DstRect = {0, 0, TILE_SIZE, TILE_SIZE};
    } else {
        m_ObjectInfo.Tile = {0, 0, m_CurrentTexture->GetWidth(),
                             m_CurrentTexture->GetHeight()};
        m_ObjectInfo.DstRect = {0, 0, m_CurrentTexture->GetWidth(),
                                m_CurrentTexture->GetHeight()};
    }
}

void Editor::ShowTextureIDs() {
    std::vector<std::string> const& texture_i_ds =
        Renderer::GetInstance()->GetTextureIDs();
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
        for (auto* obj : row) {
            objects.push_back(new GameObject(obj));
        }
    }
    m_Rooms[m_CurrentRoomID] = std::vector<GameObject*>(objects);
}

void Editor::ShowFileManager() {
    if (ImGui::BeginTabItem("File")) {
        ImGui::Text("Current room: %s", m_CurrentRoomID.c_str());

        if (ImGui::Button("Save SDFSDFSDProject", ImVec2(150, 20))) {
            SaveProject();
        }

        if (ImGui::Button("Create Room", ImVec2(150, 20))) {
            ImGui::OpenPopup("create_room");
        }

        if (ImGui::BeginPopup("create_room")) {
            static char room_name[128];
            ImGui::InputText("Input room name", room_name, sizeof(room_name));
            if (strcmp(room_name, "") != 0) {
                if (ImGui::Button("Create room", ImVec2(100, 30))) {
                    CleanLayers();
                    m_CurrentRoomID = room_name;
                    AddRoom();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Close", ImVec2(100, 30))) {
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }

        if (ImGui::Button("Save Room", ImVec2(150, 20))) {
            ImGui::OpenPopup("save_room");
        }

        if (ImGui::BeginPopup("save_room")) {
            static char room_name[128];
            ImGui::InputText("Input room name", room_name, sizeof(room_name));
            if (strcmp(room_name, "") != 0) {
                if (ImGui::Button("Save room", ImVec2(100, 30))) {
                    m_CurrentRoomID = room_name;
                    SaveRoom(m_CurrentRoomID.c_str());
                    AddRoom();
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Close", ImVec2(100, 30))) {
                    ImGui::CloseCurrentPopup();
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

                        CleanLayers();

                        m_Layers.push_back(CopyObjects(m_Rooms[id]));
                        m_CurrentRoomID = id;
                        ImGui::CloseCurrentPopup();
                    }
                }
            }
            ImGui::EndPopup();
        }

        ImGui::EndTabItem();
    }
}

void Editor::ShowAddCollider() {
    auto* collider = dynamic_cast<Collider*>(m_CurrentObject);
    if (collider == nullptr && ImGui::TreeNode("Add CollisionBox")) {

        ImGui::InputInt("Set collider width", &m_ObjectInfo.CollisionBox.w);

        ImGui::InputInt("Set collider height", &m_ObjectInfo.CollisionBox.h);

        if (ImGui::Button("Add collider", ImVec2(100, 30))) {
            collider = new Collider(m_CurrentObject);
            collider->GetCollisionBox().Set(
                m_CurrentObject->GetX(), m_CurrentObject->GetY(),
                m_ObjectInfo.CollisionBox.w, m_ObjectInfo.CollisionBox.h);
            DeleteObject(m_CurrentObject);
            m_Layers[m_CurrentLayer].push_back(collider);
            m_CurrentObject = collider;
        }
        ImGui::TreePop();
    }
}

void Editor::ShowAddAnimation() {
    char animation_label[LABEL_LEN + 1];
    char button_label[LABEL_LEN + 1];
    char status_label[LABEL_LEN + 1];

    if (m_CurrentObject->GetAnimation() == nullptr) {
        snprintf(animation_label, LABEL_LEN, "%s idle animation", "Add");
        snprintf(button_label, LABEL_LEN, "%s animation", "Add");
        snprintf(status_label, LABEL_LEN, "Successfully %s animation", "added");
    } else {
        snprintf(animation_label, LABEL_LEN, "%s idle animation", "Change");
        snprintf(button_label, LABEL_LEN, "%s animation", "Change");
        snprintf(status_label, LABEL_LEN, "Successfully %s animation",
                 "changed");
    }

    static int show_status_timer = 0;

    if (ImGui::TreeNode(animation_label)) {

        ImGui::InputInt("Set sprite row", &m_ObjectInfo.Animation.Tile.row);
        ImGui::InputInt("Set sprite col", &m_ObjectInfo.Animation.Tile.col);
        ImGui::InputInt("Set sprite width", &m_ObjectInfo.Animation.Tile.w);
        ImGui::InputInt("Set sprite height", &m_ObjectInfo.Animation.Tile.h);

        ImGui::InputInt("Set frame count", &m_ObjectInfo.Animation.FrameCount);
        ImGui::InputInt("Set animation speed",
                        &m_ObjectInfo.Animation.AnimationSpeed);

        if (ImGui::Button(button_label, ImVec2(100, 30))) {
            if (m_CurrentObject->GetAnimation() == nullptr) {
                m_CurrentObject->SetAnimation(new Animation());
            }

            m_CurrentObject->GetAnimation()->SetProps(
                {m_CurrentObject->GetTextureID(), m_ObjectInfo.Animation.Tile,
                 m_ObjectInfo.Animation.FrameCount,
                 m_ObjectInfo.Animation.AnimationSpeed});
            show_status_timer = 300;
        }
        ImGui::TreePop();
    }
    if (show_status_timer > 0) {
        ImGui::Text("%s", status_label);
        show_status_timer--;
    }
}

void Editor::ShowObjectEditor() {
    if (ImGui::BeginTabItem("Object Editor")) {
        if (ImGui::TreeNode("Object list")) {
            for (auto it = m_Layers[m_CurrentLayer].begin();
                 it != m_Layers[m_CurrentLayer].end(); it++) {
                if (ImGui::Button((*it)->GetID().c_str(), ImVec2(100, 30))) {
                    m_CurrentObject = *it;
                    m_Layers[m_CurrentLayer].erase(it);
                    m_Layers[m_CurrentLayer].insert(
                        m_Layers[m_CurrentLayer].end(), m_CurrentObject);
                }
            }
            ImGui::TreePop();
        }

        if (!m_SelectedObjects.empty()) {
            if (ImGui::Button("Rotate left", ImVec2(100, 30))) {
                for (const auto& obj : m_SelectedObjects) {
                    obj->GetRotation() -= 90.0F;
                    obj->SetRotation(static_cast<int>(obj->GetRotation()) %
                                     360);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Rotate right", ImVec2(100, 30))) {
                for (const auto& obj : m_SelectedObjects) {
                    obj->GetRotation() += 90.0F;
                    obj->SetRotation(static_cast<int>(obj->GetRotation()) %
                                     360);
                }
            }

            if (ImGui::Button("Delete objects", ImVec2(100, 30))) {

                for (const auto& obj : m_SelectedObjects) {
                    DeleteObject(obj);
                }
                m_SelectedObjects.clear();
            }

        } else if (m_CurrentObject != nullptr) {
            ImGui::Text("Selected object: %s",
                        m_CurrentObject->GetID().c_str());
            ImGui::Text("Texture:");
            Texture* obj_texture = Renderer::GetInstance()->GetTexture(
                m_CurrentObject->GetTextureID());

            ImVec2 size;
            ImVec2 uv0;
            ImVec2 uv1;

            if (auto* tile_map = dynamic_cast<TileMap*>(obj_texture)) {
                size = {static_cast<float>(tile_map->GetTileSize()) * 10,
                        static_cast<float>(tile_map->GetTileSize()) * 10};
                uv0 = ImVec2(m_CurrentObject->GetTilePos().col /
                                 static_cast<float>(tile_map->GetCols()),
                             m_CurrentObject->GetTilePos().row /
                                 static_cast<float>(tile_map->GetRows()));
                uv1 = ImVec2((m_CurrentObject->GetTilePos().col + 1) /
                                 static_cast<float>(tile_map->GetCols()),
                             (m_CurrentObject->GetTilePos().row + 1) /
                                 static_cast<float>(tile_map->GetRows()));
            } else {
                size =
                    ImVec2(obj_texture->GetWidth(), obj_texture->GetHeight());
                uv0 = {0, 0};
                uv1 = {1, 1};
            }

            ImGui::Image((void*)obj_texture->GetTexture(), size, uv0, uv1);

            ImGui::SliderFloat("X position", &m_CurrentObject->GetX(), 0,
                               LEVEL_WIDTH - m_CurrentObject->GetWidth());
            ImGui::SliderFloat("Y position", &m_CurrentObject->GetY(), 0,
                               LEVEL_HEIGHT - m_CurrentObject->GetHeight());

            ImGui::SliderInt("Width", &m_CurrentObject->GetWidth(), 0,
                             LEVEL_WIDTH);
            ImGui::SliderInt("Height", &m_CurrentObject->GetHeight(), 0,
                             LEVEL_HEIGHT);

            if (ImGui::Button("Rotate left", ImVec2(100, 30))) {
                m_CurrentObject->GetRotation() -= 90.0F;
                m_CurrentObject->GetRotation() =
                    static_cast<int>(m_CurrentObject->GetRotation()) % 360;
            }
            ImGui::SameLine();
            if (ImGui::Button("Rotate right", ImVec2(100, 30))) {
                m_CurrentObject->GetRotation() += 90.0F;
                m_CurrentObject->GetRotation() =
                    static_cast<int>(m_CurrentObject->GetRotation()) % 360;
            }

            if (ImGui::Button("Delete object", ImVec2(100, 30))) {
                DeleteObject(m_CurrentObject);
            }

            ShowAddCollider();
            ShowAddAnimation();
        }

        ImGui::Text("Snap to grid: ");
        ImGui::SameLine();
        const char* snap_to_grid = m_ObjectInfo.SnapToGrid ? "True" : "False";
        if (ImGui::Button(snap_to_grid, ImVec2(80, 30))) {
            m_ObjectInfo.SnapToGrid = !m_ObjectInfo.SnapToGrid;
        }

        std::string const select_label =
            m_EditState.EditMode == EditMode::SELECT ? "Stop Select"
                                                     : "Begin Select";
        if (ImGui::Button(select_label.c_str())) {
            m_CurrentObject = nullptr;
            std::cout << "edit state " << static_cast<int>(m_EditState.EditMode) << std::endl;
            m_EditState.EditMode = m_EditState.EditMode == EditMode::SELECT
                                       ? EditMode::NONE
                                       : EditMode::SELECT;
            std::cout << "state chagned " << static_cast<int>(m_EditState.EditMode) << std::endl;
        }

        if (ImGui::Button("Deselect")) {
            m_SelectedObjects.clear();
            StopEditing();
        }

        std::string const erase_label = m_EditState.EditMode == EditMode::ERASE
                                            ? "Stop Erase"
                                            : "Begin Erase";
        if (ImGui::Button(erase_label.c_str())) {
            m_EditState.EditMode = m_EditState.EditMode == EditMode::ERASE
                                       ? EditMode::NONE
                                       : EditMode::ERASE;
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

                m_CurrentTexture =
                    (is_tile_map)
                        ? Renderer::GetInstance()->AddTileMap(
                              texture_id, filepath, tile_size, rows, cols)
                        : Renderer::GetInstance()->AddTexture(texture_id,
                                                              filepath);

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
            ImGui::Text("ERROR: failed to load texture from %s",
                        invalid_filepath);
        }

        ImGui::EndTabItem();
    }
}

void Editor::AddObject(float x, float y) {
    GameObject* new_object;

    // safety check
    if (m_CurrentTexture == nullptr)
        return;
    
    // std::cout << "Current texture: " << m_CurrentTexture << '\n';
    std::string obj_id = m_CurrentTexture->GetID();
    obj_id += std::to_string(m_CurrentTexture->GetObjectCount());

    m_ObjectInfo.DstRect.x = x;
    m_ObjectInfo.DstRect.y = y;

    Properties props(m_CurrentTexture->GetID(), m_ObjectInfo.Tile,
                     m_ObjectInfo.DstRect, m_ObjectInfo.Rotation, obj_id);

    switch (m_ObjectInfo.type) {
        case ObjectType::Base:
            new_object = new GameObject(props);
            break;
        case ObjectType::Player:
            new_object = new Player(props);
            break;
        case ObjectType::Enemy:
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

void Editor::PrintLayer() {
    for (GameObject* obj : m_Layers[m_CurrentLayer]) {
        std::cout << obj->GetID() << "\tTILE: " << obj->GetTilePos().col << ","
                  << obj->GetTilePos().row << "\tLAYER: " << m_CurrentLayer
                  << std::endl;
    }
}

void Editor::PrintLayer(int row, int col) {
    for (GameObject* obj : m_Layers[m_CurrentLayer]) {
        std::cout << obj->GetID() << "\tTILE: " << obj->GetTilePos().row << ","
                  << obj->GetTilePos().col << "\tW" << obj->GetTilePos().w
                  << "\tH" << obj->GetTilePos().h
                  << "\tLAYER: " << m_CurrentLayer << std::endl;
    }
}

void Editor::DeleteObject(GameObject* obj) {
    // if (m_Rooms.find(m_CurrentRoomID) != m_Rooms.end()) {
    //     std::vector<GameObject*>& room = m_Rooms[m_CurrentRoomID];
    //     auto it = std::find(room.begin(), room.end(), obj);
    //     if (it != room.end()) {
    //         room.erase(it);
    //     }
    // }
    for (auto& layer : m_Layers) {
        auto it = std::find(layer.begin(), layer.end(), obj);
        if (it != layer.end()) {
            SDL_Log("found obj in layer: %s", (*it)->GetID().c_str());
            layer.erase(it);
            SDL_Log("layer size: %lu", layer.size());
        }
    }

    delete obj;
    obj = nullptr;
}

void Editor::ShowBuildPlayer() {
    ImGui::Text("Showing build player stuff");
}

void Editor::ShowBuildEnemy() {

    ImGui::InputInt("Input perception width", &m_EnemyInfo.PerceptionWidth);
    ImGui::InputInt("Input perception height", &m_EnemyInfo.PerceptionHeight);
}

ObjectType Editor::ShowSelectObjectType() {
    static int current_index =
        0;  // Here we store our selection data as an index.
    const char* preview_value = object_type_strs[current_index];
    if (ImGui::BeginCombo("Select object type", preview_value, 0)) {
        for (int i = 0; i < IM_ARRAYSIZE(object_type_strs); i++) {
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
    m_ObjectInfo.type = static_cast<ObjectType>(current_index + 1);
    return static_cast<ObjectType>(current_index + 1);
}

void Editor::ShowTiles(TileMap* tileMap) {
    ImGui::Text("Select tile");
    ImVec2 const size = ImVec2(tileMap->GetTileSize(), tileMap->GetTileSize());
    for (int i = 0; i < tileMap->GetRows(); i++) {
        for (int j = 0; j < tileMap->GetCols(); j++) {
            if (j != 0) {
                ImGui::SameLine();
            }

            ImVec2 const uv0 =
                ImVec2(j / static_cast<float>(tileMap->GetCols()),
                       i / static_cast<float>(tileMap->GetRows()));
            ImVec2 const uv1 =
                ImVec2((j + 1) / static_cast<float>(tileMap->GetCols()),
                       (i + 1) / static_cast<float>(tileMap->GetRows()));

            bool const is_active = tileMap->GetActiveButtons()[i][j];
            if (is_active) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(23, 30, 57, 255));
            }

            ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                                  ImVec4(23, 30, 57, 255));

            if (ImGui::ImageButton("", (ImTextureID)tileMap->GetTexture(), size,
                                   uv0, uv1) ||
                ImGui::IsItemClicked()) {
                tileMap->ClearButtons();
                m_ObjectInfo.Tile.row = i;
                m_ObjectInfo.Tile.col = j;
                tileMap->GetActiveButtons()[i][j] = !is_active;
            }

            ImGui::PopStyleColor();
            if (is_active) {
                ImGui::PopStyleColor();
            }
        }
    }
}

void Editor::ShowCreateObject() {
    if (ImGui::BeginTabItem("Create object")) {
        ShowTextureIDs();

        if (m_CurrentTexture == nullptr) {
            ImGui::Text("Please select a texture to create an object");
        } else {
            ImGui::Text("Selected texture: %s",
                        m_CurrentTexture->GetID().c_str());
            ImGui::Image((void*)m_CurrentTexture->GetTexture(),
                         ImVec2(m_CurrentTexture->GetWidth(),
                                m_CurrentTexture->GetHeight()));
            ImGui::Text("size = %d x %d", m_CurrentTexture->GetWidth(),
                        m_CurrentTexture->GetHeight());

            auto* tile_map = dynamic_cast<TileMap*>(m_CurrentTexture);
            if (tile_map != nullptr) {
                // ImGui::SliderInt("Select tile row", &m_ObjectInfo.Tile.row, 0, tileMap->GetRows() - 1);
                // ImGui::SliderInt("Select tile column", &m_ObjectInfo.Tile.col, 0, tileMap->GetCols() - 1);
                ShowTiles(tile_map);
            }

            ImGui::SeparatorText("Select dimensions");
            ImGui::SliderInt("Select destination width: ",
                             &m_ObjectInfo.DstRect.w, 0, LEVEL_WIDTH);
            ImGui::SliderInt("Select destination height: ",
                             &m_ObjectInfo.DstRect.h, 0, LEVEL_HEIGHT);

            ObjectType const type = ShowSelectObjectType();
            switch (type) {
                case ObjectType::Base:
                    break;
                case ObjectType::Player:
                    ShowBuildPlayer();
                    break;
                case ObjectType::Enemy:
                    ShowBuildEnemy();
                    break;
                default:
                    SDL_LogError(0, "Invalid object type");
                    assert(false);
            }
            std::string const draw_label =
                m_EditState.EditMode == EditMode::DRAW ? "Stop Draw"
                                                       : "Begin Draw";

            if (ImGui::Button(draw_label.c_str())) {
                m_EditState.EditMode = m_EditState.EditMode == EditMode::DRAW
                                           ? EditMode::NONE
                                           : EditMode::DRAW;
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
            char layer_label[LABEL_LEN + 1];
            snprintf(layer_label, LABEL_LEN, "Layer %d", i);
            if (ImGui::Selectable(layer_label, m_CurrentLayer == i)) {
                m_CurrentLayer = i;
                m_CurrentObject = nullptr;
            }

            char button_label[LABEL_LEN + 1];
            if (m_HiddenLayers.find(i) == m_HiddenLayers.end()) {
                snprintf(button_label, LABEL_LEN, "Hide %d", i);
                if (ImGui::Button(button_label, ImVec2(60, 20))) {
                    SDL_Log("layer %d should be hidden", i);
                    m_HiddenLayers.insert(i);
                }
            } else {
                snprintf(button_label, LABEL_LEN, "Show %d", i);
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

void Editor::Update(float /*dt*/) {
    // Now includes checks for previous keys

    // basing input off of editor acction allows us to use bindings and easier rebinds in the future
    if (m_KeyMap->CheckInputs(EditorAction::PAN_CAMERA_UP)) {
        Renderer::GetInstance()->MoveCameraY(-10.0F);
    }
    if (m_KeyMap->CheckInputs(EditorAction::PAN_CAMERA_DOWN)) {
        Renderer::GetInstance()->MoveCameraY(10.0F);
    }
    if (m_KeyMap->CheckInputs(EditorAction::PAN_CAMERA_LEFT)) {
        Renderer::GetInstance()->MoveCameraX(-10.0F);
    }
    if (m_KeyMap->CheckInputs(EditorAction::PAN_CAMERA_RIGHT)) {
        Renderer::GetInstance()->MoveCameraX(10.0F);
    }

    // Check and handle tool selection / deselection via keybinds
    // the EditorAction param is the result of a satisfied keybind input, with the EditMode param being the tool selection
    CheckForToolSelection(EditorAction::ENTER_DRAW_TOOL, EditMode::DRAW);
    CheckForToolSelection(EditorAction::ENTER_ERASE_TOOL, EditMode::ERASE);
    CheckForToolSelection(EditorAction::ENTER_TILE_SELECT_TOOL, EditMode::TILE_SELECT);
    CheckForToolSelection(EditorAction::ENTER_SELECTION_MOVE_TOOL, EditMode::DRAG_MOVE);

    // Tool deselection
    if (m_KeyMap->CheckInputs(EditorAction::EXIT_CURRENT_TOOL)) {
        StopEditing();

        // deselect all
        m_SelectedObjects.clear();
    }

    // COPY & PASTE COMBOS
    m_KeyMap->CheckInputs(EditorAction::COPY_SELECTION);
    m_KeyMap->CheckInputs(EditorAction::PASTE_SELECTION);

    // UNDO & REDO COMBOS
    m_KeyMap->CheckInputs(EditorAction::UNDO_ACTION);
    m_KeyMap->CheckInputs(EditorAction::REDO_ACTION);

    InputChecker::SetPrevFrameKeys();
}

void Editor::Render() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
    ShowObjectManager();

    ImGui::Render();

    Renderer::GetInstance()->RenderClear();
    for (int i = 0; i < m_Layers.size(); i++) {
        if (m_HiddenLayers.find(i) == m_HiddenLayers.end()) {
            for (auto* obj : m_Layers[i]) {
                obj->Update(0.0);
                obj->Draw();
                if (m_SelectedObjects.find(obj) != m_SelectedObjects.end()) {
                    std::vector<SDL_Rect> rects;
                    rects.reserve(4);
                    for (int i = 0; i < 4; i++) {
                        rects.push_back({static_cast<int>(obj->GetX()) + i,
                                         static_cast<int>(obj->GetY()) + i,
                                         obj->GetWidth(), obj->GetHeight()});
                    }
                    Renderer::GetInstance()->DrawRects(rects,
                                                       {0, 150, 255, 255});
                }
            }
        }
    }

    DrawGrid();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

    // draw cursor
    if (static_cast<int>(m_Cursor->GetCursorType()) != 0) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);  // hide default cursor

        std::string const cursor_texture_id = m_Cursor->GetTextureId(static_cast<int>(m_EditState.EditMode));

        SDL_Rect const cursor_rect = m_Cursor->UpdateAndGetRect(
            m_CursorOffsets[cursor_texture_id].first, m_CursorOffsets[cursor_texture_id].second);


        SDL_RenderCopy(Renderer::GetInstance()->GetRenderer(),
                       Renderer::GetInstance()
                           ->GetTexture(cursor_texture_id)
                           ->GetTexture(),
                       nullptr, &cursor_rect);
    }

    if ((ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) != 0) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    Renderer::GetInstance()->Render();
}

GameObject* Editor::GetObjectUnderMouse() {
    GameObject* obj = nullptr;
    auto it = m_Layers[m_CurrentLayer].end();
    while (it != m_Layers[m_CurrentLayer].begin()) {
        --it;
        if (CheckMouseOver(*it)) {
            obj = *it;
            return obj;
        }
    }
    return obj;
}

void Editor::OnMouseClicked(SDL_Event& event) {
    m_MouseInputOrigin = GetMouseTilePos();
    m_EditState.PrevCoords = m_MouseInputOrigin;

    switch(m_EditState.EditMode) {
        case EditMode::DRAW:
            HandleDrawAction();
            break;
        case EditMode::ERASE:
            HandleEraseAction();
            break;
        case EditMode::TILE_SELECT:
            // HandleTileSelectAction(false, event);
            break;
        case EditMode::DRAG_MOVE:
            HandleDragMoveAction(event);
            break;
        case EditMode::NONE:
            HandleNoToolActions(false, event);
            break;
        default:
            throw(std::runtime_error("No EditMode assigned to m_Editstate"));
    }
}

void Editor::OnMouseMoved(SDL_Event& event) {
    if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
        switch(m_EditState.EditMode) {
            case EditMode::DRAW:
                HandleDrawAction();
                break;
            case EditMode::ERASE:
                HandleEraseAction();
                break;
            case EditMode::TILE_SELECT:
                HandleTileSelectAction(true, event);
                break;
            case EditMode::DRAG_MOVE:
                HandleDragMoveAction(event);
                break;
            case EditMode::NONE:
                HandleNoToolActions(true, event);
                break;
            default:
                throw(std::runtime_error("No EditMode assigned to m_Editstate"));
        }
        m_EditState.PrevCoords = GetMouseTilePos();
        if (m_EditState.EditMode != EditMode::NONE)
            m_EditState.IsEditing = true;
    }
}

void Editor::OnMouseUp(SDL_Event& event) {

    switch(m_EditState.EditMode) {
        case EditMode::DRAW:
            // HandleDrawAction();
            break;
        case EditMode::ERASE:
            // HandleEraseAction();
            break;
        case EditMode::TILE_SELECT:
            HandleTileSelectAction(false, event);
            break;
        case EditMode::DRAG_MOVE:
            HandleDragMoveAction(event);
            break;
        case EditMode::NONE:
            HandleNoToolActions(false, event);
            break;
        default:
            throw(std::runtime_error("No EditMode assigned to m_Editstate"));
    }
    m_EditState.IsEditing = false;
    m_EditState.PrevCoords = GetMouseTilePos();
}

void Editor::SnapToGrid(float x, float y, GameObject* obj) {
    float next_x = (static_cast<int>(x + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;
    float next_y = (static_cast<int>(y + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;
    obj->SetX(next_x);
    obj->SetY(next_y);
}

std::pair<float, float> Editor::SnapToGrid(float x, float y) {
    float next_x;
    float next_y;
    next_x = (static_cast<int>(x + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;
    next_y = (static_cast<int>(y + TILE_SIZE / 2) / TILE_SIZE) * TILE_SIZE;

    return {next_x, next_y};
}

std::pair<int, int> Editor::PixelToTilePos(float x, float y) {
    int row, col;
    row = y / TILE_SIZE;
    col = x / TILE_SIZE;

    return {row, col};
}

TileCoords Editor::PixelToTileCoords(float x, float y) {
    int row, col;
    row = y / TILE_SIZE;
    col = x / TILE_SIZE;

    return {row, col};
}

void Editor::Events() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        ImGuiIO const& io = ImGui::GetIO();
        if (io.WantCaptureKeyboard || io.WantCaptureMouse) {
            return;
        }
        switch (event.type) {
            case SDL_QUIT:
                Quit();
                return;
            case SDL_KEYDOWN:
                InputChecker::SetKeyPressed(event.key.keysym.sym, true);
                break;
            case SDL_KEYUP:
                InputChecker::SetKeyPressed(event.key.keysym.sym, false);
                break;
            case SDL_MOUSEBUTTONDOWN:
                InputChecker::SetMouseButtonPressed(event.button.button, true);
                OnMouseClicked(event);
                break;
            case SDL_MOUSEBUTTONUP:
                OnMouseUp(event);
                InputChecker::SetMouseButtonPressed(event.button.button, false);
                break;
            case SDL_MOUSEMOTION:
                OnMouseMoved(event);
                InputChecker::UpdateMousePosition(event.motion.x,
                                                  event.motion.y);
                break;
        }
    }
}

// removes boilerplate needed to handle tool selection via keybinds.
void Editor::CheckForToolSelection(EditorAction editor_action, EditMode edit_mode) {
    if (m_KeyMap->CheckInputs(editor_action)) {
        m_EditState.EditMode = m_EditState.EditMode != edit_mode ? edit_mode : EditMode::NONE;
        m_Cursor->SetCursor(m_EditState.EditMode);
    }
}

// x and y are tile coords
bool Editor::SelectTile(int row, int col) {
    bool selectedOrDeselectedATile = false;

    for (GameObject* obj : m_Layers[m_CurrentLayer]) {
        if (obj == nullptr) continue;

        std::pair<int, int> obj_tile_coords = PixelToTilePos(obj->GetX(), obj->GetY());

        if (obj_tile_coords.first == row && obj_tile_coords.second == col) {
            selectedOrDeselectedATile = true;

            if (m_SelectedObjects.contains(obj)) {
                if (!m_EditState.IsEditing) {
                    m_SelectedObjects.erase(obj);                    
                }
            } else { m_SelectedObjects.insert(obj); }
        }
    }

    return selectedOrDeselectedATile;
}

void Editor::HandleDrawAction() {
    const auto [x,y] = GetMousePixelPos();

    if (m_EditState.IsEditing && x == m_EditState.PrevX && y == m_EditState.PrevY)
        return;

    AddObject(x, y);

    m_EditState.PrevX = x;
    m_EditState.PrevY = y;
    m_EditState.IsEditing = true;

}

void Editor::HandleEraseAction() {
    const auto [x,y] = GetMousePixelPos();

    if (m_EditState.IsEditing && x == m_EditState.PrevX && y == m_EditState.PrevY)
        return;

    // delete all objects on mousedover tile, in current layer.
    std::pair<int, int> tile_coords = PixelToTilePos(x, y);

    // create list of objects to delete on that tile and layer
    std::vector<GameObject*> objects_to_delete;

    for (GameObject* obj : m_Layers[m_CurrentLayer]) {
        std::pair<int, int> curr_tile_coords =
            PixelToTilePos(obj->GetX(), obj->GetY());

        if (curr_tile_coords.first == tile_coords.first &&
            curr_tile_coords.second == tile_coords.second) {
            objects_to_delete.push_back(obj);
        }
    }

    // delete all the objects
    for (GameObject* obj : objects_to_delete) {
        DeleteObject(obj);
    }

    
}

void Editor::HandleNoToolActions(bool mouse_moved, SDL_Event& event) {
    TileCoords mouse_tile_coords = GetMouseTilePos();
    // do we need to drag anything?
        // conditions for this:
            // must have some tiles selected, must have mouse over a selected tile
    if (!m_SelectedObjects.empty() && IsMouseOverASelectedTile(mouse_tile_coords) && mouse_moved) {
        HandleDragMoveAction(event);
        return;
    }
    

    // do we need to select anything?
        // conditions for this:
            // mouse over unselected tile, mouse_moved == false
        // deselect all, then make selection

    // if the mouse moved, then no select takes place
    if (mouse_moved)
        return;

    if (event.button.type == SDL_MOUSEBUTTONDOWN) {
        m_MouseInputOrigin = mouse_tile_coords;        
    }

    if (event.button.type == SDL_MOUSEBUTTONUP) {
        bool clickedEmptyTile = false;

        // toggle tile selection
        if (m_MouseInputOrigin == mouse_tile_coords)
            clickedEmptyTile = !SelectTile(mouse_tile_coords.row, mouse_tile_coords.col);

        // deslect all
        if (clickedEmptyTile)
            m_SelectedObjects.clear();

        m_EditState.IsEditing = false;
    }
}

// tile select
void Editor::HandleTileSelectAction(bool mouse_moved, SDL_Event& event) {
    // update if needed
    if (m_EditState.EditMode != EditMode::TILE_SELECT)
        UpdateEditMode(EditMode::TILE_SELECT, true);

    TileCoords mouse_tile_coords = GetMouseTilePos();
    // std::cout << "prev " << m_EditState.PrevCoords.col << ", " << m_EditState.PrevCoords.row 
        // << "  \tcurr " << mouse_tile_coords.col <<  ", " << mouse_tile_coords.row 
        // << "  \tisediting " << m_EditState.IsEditing << std::endl;
    if (m_EditState.PrevCoords == mouse_tile_coords && m_EditState.IsEditing)
        return;

    // if found nothing deselect all && change edit mode
    bool foundObj = SelectTile(mouse_tile_coords.row, mouse_tile_coords.col);
    if (!foundObj && !mouse_moved) {
        m_SelectedObjects.clear();
        StopEditing();
    }

    // stop editing on mouse up
    if (event.button.type == SDL_MOUSEBUTTONUP)
        m_EditState.IsEditing = false;
}

void Editor::HandleDragMoveAction(SDL_Event& event) {
    const auto [x,y] = GetMousePixelPos();
    TileCoords mouse_tile_coords = GetMouseTilePos();

    // update if needed
    if (m_EditState.EditMode != EditMode::DRAG_MOVE)
        UpdateEditMode(EditMode::DRAG_MOVE, true);

    // finish move on mouse up
    if (event.button.type == SDL_MOUSEBUTTONUP) {
        for (const auto& obj : m_SelectedObjects) 
            SnapToGrid(x, y, obj);

        StopEditing();
        return;
    }

    // on mouse move, move selection
    if (event.button.type == SDL_MOUSEMOTION) {
        float const dx = event.button.x - InputChecker::GetMouseX();
        float const dy = event.button.y - InputChecker::GetMouseY();
        for (const auto& obj : m_SelectedObjects)
            MoveObject(obj, dx, dy);
    }

}

bool Editor::IsMouseOverASelectedTile(TileCoords coords) {
    for (const auto& obj : m_SelectedObjects) {
        TileCoords obj_coords = PixelToTileCoords(obj->GetX(), obj->GetY());
        
        if (obj_coords == coords) 
            return true;
    }
    return false;
}

std::tuple<float,float>Editor::GetMousePixelPos() {
    float const x = ((InputChecker::GetMouseX() + Renderer::GetInstance()->GetCameraX()) / TILE_SIZE) * TILE_SIZE;
    float const y = ((InputChecker::GetMouseY() + Renderer::GetInstance()->GetCameraY()) / TILE_SIZE) * TILE_SIZE;

    return {x,y};
}

TileCoords Editor::GetMouseTilePos() {
    // pixel pos
    float const pixelX = ((InputChecker::GetMouseX() + Renderer::GetInstance()->GetCameraX()) / TILE_SIZE) * TILE_SIZE;
    float const pixelY = ((InputChecker::GetMouseY() + Renderer::GetInstance()->GetCameraY()) / TILE_SIZE) * TILE_SIZE;

    // conversion
    int row = pixelY / TILE_SIZE;
    int col = pixelX / TILE_SIZE;

    return {row, col};
}

void Editor::UpdateEditMode(EditMode mode, bool isEditing) {
    m_EditState.IsEditing = isEditing;
    m_EditState.EditMode = mode;
    m_Cursor->SetCursor(m_EditState.EditMode);
}

void Editor::StopEditing() {
    m_EditState.IsEditing = false;
    m_EditState.EditMode = EditMode::NONE;
    m_Cursor->SetCursor(m_EditState.EditMode);
}

bool Editor::LoadEditorTextures() {
    tinyxml2::XMLDocument doc;
    std::string const textures_path = "../assets/editor/editor_textures.xml";

    // check for loading error for xml
    tinyxml2::XMLError const error = doc.LoadFile(textures_path.c_str());
    if (error != tinyxml2::XML_SUCCESS) {
        SDL_LogError(0, "Could not load textures");
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("Root");
    tinyxml2::XMLElement* curr_texture = root->FirstChildElement("Texture");

    std::string type;
    std::string id;
    std::string texture_path;

    while (curr_texture != nullptr) {
        type = curr_texture->Attribute("type");
        id = curr_texture->FirstChildElement("ID")->GetText();

        if (curr_texture->FirstChildElement("FilePath")->GetText() != nullptr) {
            texture_path = curr_texture->FirstChildElement("FilePath")->GetText();
            Renderer::GetInstance()->AddTexture(id, texture_path);

            int offsetX = std::stoi(curr_texture->FirstChildElement("OffsetX")->GetText());
            int offsetY = std::stoi(curr_texture->FirstChildElement("OffsetY")->GetText());

            m_CursorOffsets[id] = {offsetX, offsetY};
        }

        curr_texture = curr_texture->NextSiblingElement("Texture");
    }

    return true;
}

#if EDITOR == 1
Application* CreateApplication() {
    return new Editor();
}
#endif