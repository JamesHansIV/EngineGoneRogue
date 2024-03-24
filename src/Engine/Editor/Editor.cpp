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
#include <queue>

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
    for (int i = 0; i < LevelWidth; i += TileSize) {
        Renderer::GetInstance()->DrawLine(i, 0, i, LevelHeight);
    }
    for (int i = 0; i < LevelHeight; i += TileSize) {
        Renderer::GetInstance()->DrawLine(0, i, LevelWidth, i);
    }
    // std::cout << "LEVEL r,c " << LEVEL_ROWS << ", " << LEVEL_COLS << std::endl;
    // exit(0);
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
    m_rooms = Application::m_rooms;
    m_layers.emplace_back();

    // load editor textures
    LoadEditorTextures();

    // create keymap
    m_key_map = new KeyMap();

    // create cursor
    m_cursor = new Cursor();
}

Editor::~Editor() {
    //TODO: check if cleanup is successful
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    CleanLayers();

    delete m_key_map;
    delete m_cursor;
}

void Editor::CleanLayers() {
    m_hidden_layers.clear();
    m_current_texture = nullptr;
    m_current_object = nullptr;
    for (auto& m_layer : m_layers) {
        for (auto* obj : m_layer) {
            delete obj;
        }
    }
    m_layers.clear();
}

void Editor::SaveRoom(const char* roomName) {
    std::vector<GameObject*> objects;

    for (const auto& layer : m_layers) {
        for (auto* obj : layer) {
            objects.push_back(obj);
        }
    }

    char file_path[FilepathLen + 1];
    snprintf(file_path, FilepathLen, "../assets/projects/%s/rooms/%s.xml",
             m_project_name.c_str(), roomName);

    int const success = SaveObjects(file_path, objects);
    SDL_Log("Saving room a success: %d", success);
}

void Editor::SaveProject() {
    CreateProjectFolder();

    char dst_path[FilepathLen + 1];
    snprintf(dst_path, FilepathLen, "../assets/projects/%s/textures.xml",
             GetProjectName().c_str());
    Renderer::GetInstance()->SaveTextures(dst_path);

    for (const auto& room : m_rooms) {
        SaveRoom(room.first.c_str());
    }
}

void Editor::CreateProjectFolder() {
    struct dirent* entry;
    DIR* dp;

    char project_path[FilepathLen + 1];
    snprintf(project_path, FilepathLen, "../assets/projects/%s",
             m_project_name.c_str());
    dp = opendir(project_path);
    if (dp == nullptr) {
        char command[FilepathLen + 1];
        snprintf(command, FilepathLen, "mkdir %s", project_path);
        system(command);
    }
    closedir(dp);
}

void Editor::SetObjectInfo() {
    auto* tile_map = dynamic_cast<TileMap*>(m_current_texture);
    if (tile_map != nullptr) {
        m_object_info.Tile = {0, 0, tile_map->GetTileSize(),
                              tile_map->GetTileSize()};
        m_object_info.DstRect = {0, 0, TileSize, TileSize};
    } else {
        m_object_info.Tile = {0, 0, m_current_texture->GetWidth(),
                              m_current_texture->GetHeight()};
        m_object_info.DstRect = {0, 0, m_current_texture->GetWidth(),
                                 m_current_texture->GetHeight()};
        m_object_info.CollisionBox = {0, 0, m_object_info.DstRect.w,
                                      m_object_info.DstRect.h};
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
                    m_current_texture = Renderer::GetInstance()->GetTexture(id);
                    SetObjectInfo();
                }
            }
            ImGui::TreePop();
        }
    }
}

void Editor::AddRoom() {
    std::vector<GameObject*> objects;
    for (const auto& row : m_layers) {
        for (auto* obj : row) {
            objects.push_back(new GameObject(obj));
        }
    }
    m_rooms[m_current_room_id] = std::vector<GameObject*>(objects);
}

void Editor::ShowFileManager() {
    if (ImGui::BeginTabItem("File")) {
        ImGui::Text("Current room: %s", m_current_room_id.c_str());

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
                    m_current_room_id = room_name;
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
                    m_current_room_id = room_name;
                    SaveRoom(m_current_room_id.c_str());
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
            for (const auto& item : m_rooms) {
                std::string const id = item.first;
                SDL_Log("Room: %s", id.c_str());
                if (strcmp(id.c_str(), "") != 0) {
                    if (ImGui::Button(id.c_str(), ImVec2(100, 30))) {

                        CleanLayers();

                        m_layers.push_back(CopyObjects(m_rooms[id]));
                        m_current_room_id = id;
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
    auto* collider = dynamic_cast<Collider*>(m_current_object);
    if (collider == nullptr && ImGui::TreeNode("Add Collider")) {

        ImGui::InputInt("Set collider width", &m_object_info.CollisionBox.w);

        ImGui::InputInt("Set collider height", &m_object_info.CollisionBox.h);

        if (ImGui::Button("Add collider", ImVec2(100, 30))) {
            collider = new Collider(m_current_object);
            collider->GetCollisionBox().Set(
                m_current_object->GetX(), m_current_object->GetY(),
                m_object_info.CollisionBox.w, m_object_info.CollisionBox.h);
            DeleteObject(m_current_object);
            m_layers[m_current_layer].push_back(collider);
            m_current_object = collider;
        }
        ImGui::TreePop();
    }
}

void Editor::ShowAddAnimation() {
    char animation_label[LabelLen + 1];
    char button_label[LabelLen + 1];
    char status_label[LabelLen + 1];

    if (m_current_object != nullptr &&
        m_current_object->GetAnimation() == nullptr) {
        snprintf(animation_label, LabelLen, "%s idle animation", "Add");
        snprintf(button_label, LabelLen, "%s animation", "Add");
        snprintf(status_label, LabelLen, "Successfully %s animation", "added");
    } else {
        snprintf(animation_label, LabelLen, "%s idle animation", "Change");
        snprintf(button_label, LabelLen, "%s animation", "Change");
        snprintf(status_label, LabelLen, "Successfully %s animation",
                 "changed");
    }

    static int show_status_timer = 0;

    if (ImGui::TreeNode(animation_label)) {

        ImGui::InputInt("Set sprite row", &m_object_info.Animation.Tile.row);
        ImGui::InputInt("Set sprite col", &m_object_info.Animation.Tile.col);
        ImGui::InputInt("Set sprite width", &m_object_info.Animation.Tile.w);
        ImGui::InputInt("Set sprite height", &m_object_info.Animation.Tile.h);

        ImGui::InputInt("Set frame count", &m_object_info.Animation.FrameCount);
        ImGui::InputInt("Set animation speed",
                        &m_object_info.Animation.AnimationSpeed);

        if (ImGui::Button(button_label, ImVec2(100, 30))) {
            if (m_current_object->GetAnimation() == nullptr) {
                m_current_object->SetAnimation(new Animation());
            }

            m_current_object->GetAnimation()->SetProps(
                {m_current_object->GetTextureID(), m_object_info.Animation.Tile,
                 m_object_info.Animation.FrameCount,
                 m_object_info.Animation.AnimationSpeed});
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
            for (auto it = m_layers[m_current_layer].begin();
                 it != m_layers[m_current_layer].end(); it++) {
                if (ImGui::Button((*it)->GetID().c_str(), ImVec2(100, 30))) {
                    m_current_object = *it;
                    m_layers[m_current_layer].erase(it);
                    m_layers[m_current_layer].insert(
                        m_layers[m_current_layer].end(), m_current_object);
                }
            }
            ImGui::TreePop();
        }

        if (!m_selected_objects.empty()) {
            if (ImGui::Button("Rotate left", ImVec2(100, 30))) {
                for (const auto& obj : m_selected_objects) {
                    obj->GetRotation() -= 90.0F;
                    obj->SetRotation(static_cast<int>(obj->GetRotation()) %
                                     360);
                }
            }
            ImGui::SameLine();
            if (ImGui::Button("Rotate right", ImVec2(100, 30))) {
                for (const auto& obj : m_selected_objects) {
                    obj->GetRotation() += 90.0F;
                    obj->SetRotation(static_cast<int>(obj->GetRotation()) %
                                     360);
                }
            }

            if (ImGui::Button("Delete objects", ImVec2(100, 30))) {

                for (const auto& obj : m_selected_objects) {
                    DeleteObject(obj);
                }
                m_selected_objects.clear();
            }

            bool addColliders = true;
            for (auto* obj : m_selected_objects) {
                auto* c = dynamic_cast<Collider*>(obj);
                if (c != nullptr) {
                    addColliders = false;
                }
            }
            if (addColliders &&
                ImGui::Button("Add Colliders", ImVec2(150, 30))) {
                Collider* c = nullptr;
                for (auto* obj : m_selected_objects) {
                    c = new Collider(obj);
                    DeleteObject(obj);
                    m_layers[m_current_layer].push_back(c);
                }
                m_selected_objects.clear();
            }

        } else if (m_current_object != nullptr) {
            ImGui::Text("Selected object: %s",
                        m_current_object->GetID().c_str());
            ImGui::Text("Texture:");
            Texture* obj_texture = Renderer::GetInstance()->GetTexture(
                m_current_object->GetTextureID());

            ImVec2 size;
            ImVec2 uv0;
            ImVec2 uv1;

            if (auto* tile_map = dynamic_cast<TileMap*>(obj_texture)) {
                size = {static_cast<float>(tile_map->GetTileSize()) * 10,
                        static_cast<float>(tile_map->GetTileSize()) * 10};
                uv0 = ImVec2(m_current_object->GetTilePos().col /
                                 static_cast<float>(tile_map->GetCols()),
                             m_current_object->GetTilePos().row /
                                 static_cast<float>(tile_map->GetRows()));
                uv1 = ImVec2((m_current_object->GetTilePos().col + 1) /
                                 static_cast<float>(tile_map->GetCols()),
                             (m_current_object->GetTilePos().row + 1) /
                                 static_cast<float>(tile_map->GetRows()));
            } else {
                size =
                    ImVec2(obj_texture->GetWidth(), obj_texture->GetHeight());
                uv0 = {0, 0};
                uv1 = {1, 1};
            }

            ImGui::Image((void*)obj_texture->GetTexture(), size, uv0, uv1);

            ImGui::SliderFloat("X position", &m_current_object->GetX(), 0,
                               LevelWidth - m_current_object->GetWidth());
            ImGui::SliderFloat("Y position", &m_current_object->GetY(), 0,
                               LevelHeight - m_current_object->GetHeight());

            ImGui::SliderInt("Width", &m_current_object->GetWidth(), 0,
                             LevelWidth);
            ImGui::SliderInt("Height", &m_current_object->GetHeight(), 0,
                             LevelHeight);

            if (ImGui::Button("Rotate left", ImVec2(100, 30))) {
                m_current_object->GetRotation() -= 90.0F;
                m_current_object->GetRotation() =
                    static_cast<int>(m_current_object->GetRotation()) % 360;
            }
            ImGui::SameLine();
            if (ImGui::Button("Rotate right", ImVec2(100, 30))) {
                m_current_object->GetRotation() += 90.0F;
                m_current_object->GetRotation() =
                    static_cast<int>(m_current_object->GetRotation()) % 360;
            }

            if (ImGui::Button("Delete object", ImVec2(100, 30))) {
                DeleteObject(m_current_object);
            }

            ShowAddCollider();
            ShowAddAnimation();
        }

        ImGui::Text("Snap to grid: ");
        ImGui::SameLine();
        const char* snap_to_grid = m_object_info.SnapToGrid ? "True" : "False";
        if (ImGui::Button(snap_to_grid, ImVec2(80, 30))) {
            m_object_info.SnapToGrid = !m_object_info.SnapToGrid;
        }

        std::string const select_label =
            m_edit_state.EditMode == EditMode::SELECT ? "Stop Select"
                                                      : "Begin Select";
        if (ImGui::Button(select_label.c_str())) {
            m_current_object = nullptr;
            m_edit_state.EditMode = m_edit_state.EditMode == EditMode::SELECT
                                        ? EditMode::NONE
                                        : EditMode::SELECT;
        }

        if (ImGui::Button("Deselect")) {
            m_selected_objects.clear();
            StopEditing();
        }

        std::string const erase_label = m_edit_state.EditMode == EditMode::ERASE
                                            ? "Stop Erase"
                                            : "Begin Erase";
        if (ImGui::Button(erase_label.c_str())) {
            m_edit_state.EditMode = m_edit_state.EditMode == EditMode::ERASE
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

                m_current_texture =
                    (is_tile_map)
                        ? Renderer::GetInstance()->AddTileMap(
                              texture_id, filepath, tile_size, rows, cols)
                        : Renderer::GetInstance()->AddTexture(texture_id,
                                                              filepath);

                if (m_current_texture == nullptr) {
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
    if (m_current_texture == nullptr) {
        return;
    }

    // std::cout << "Current texture: " << m_CurrentTexture << '\n';
    std::string obj_id = m_current_texture->GetID();
    obj_id += std::to_string(m_current_texture->GetObjectCount());

    m_object_info.DstRect.x = x;
    m_object_info.DstRect.y = y;

    Properties props(m_current_texture->GetID(), m_object_info.Tile,
                     m_object_info.DstRect, m_object_info.Rotation, obj_id);

    switch (m_object_info.type) {
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
    m_current_texture->IncObjectCount();
    m_layers[m_current_layer].push_back(new_object);
}

void Editor::PrintLayer() {
    for (GameObject* obj : m_layers[m_current_layer]) {
        std::cout << obj->GetID() << "\tTILE: " << obj->GetTilePos().col << ","
                  << obj->GetTilePos().row << "\tLAYER: " << m_current_layer
                  << '\n';
    }
}

void Editor::PrintLayer(int /*row*/, int /*col*/) {
    for (GameObject* obj : m_layers[m_current_layer]) {
        std::cout << obj->GetID() << "\tTILE: " << obj->GetTilePos().row << ","
                  << obj->GetTilePos().col << "\tW" << obj->GetTilePos().w
                  << "\tH" << obj->GetTilePos().h
                  << "\tLAYER: " << m_current_layer << '\n';
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
    if (m_current_object == obj) {
        m_current_object = nullptr;
    }
    for (auto& layer : m_layers) {
        auto it = std::find(layer.begin(), layer.end(), obj);
        if (it != layer.end()) {
            SDL_Log("found obj in layer: %s", (*it)->GetID().c_str());
            layer.erase(it);
            SDL_Log("layer size: %lu", layer.size());
        }
    }

    delete obj;
    SDL_Log("deleted obj");
}

void Editor::ShowBuildPlayer() {
    ImGui::Text("Showing build player stuff");
}

void Editor::ShowBuildEnemy() {

    ImGui::InputInt("Input perception width", &m_enemy_info.PerceptionWidth);
    ImGui::InputInt("Input perception height", &m_enemy_info.PerceptionHeight);
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
    m_object_info.type = static_cast<ObjectType>(current_index + 1);
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
                m_object_info.Tile.row = i;
                m_object_info.Tile.col = j;
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

        if (m_current_texture == nullptr) {
            ImGui::Text("Please select a texture to create an object");
        } else {
            ImGui::Text("Selected texture: %s",
                        m_current_texture->GetID().c_str());
            ImGui::Image((void*)m_current_texture->GetTexture(),
                         ImVec2(m_current_texture->GetWidth(),
                                m_current_texture->GetHeight()));
            ImGui::Text("size = %d x %d", m_current_texture->GetWidth(),
                        m_current_texture->GetHeight());

            auto* tile_map = dynamic_cast<TileMap*>(m_current_texture);
            if (tile_map != nullptr) {
                // ImGui::SliderInt("Select tile row", &m_ObjectInfo.Tile.row, 0, tileMap->GetRows() - 1);
                // ImGui::SliderInt("Select tile column", &m_ObjectInfo.Tile.col, 0, tileMap->GetCols() - 1);
                ShowTiles(tile_map);
            }

            ImGui::SeparatorText("Select dimensions");
            ImGui::SliderInt("Select destination width: ",
                             &m_object_info.DstRect.w, 0, LevelWidth);
            ImGui::SliderInt("Select destination height: ",
                             &m_object_info.DstRect.h, 0, LevelHeight);
            m_object_info.CollisionBox = {0, 0, m_object_info.DstRect.w,
                                          m_object_info.DstRect.h};

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
                m_edit_state.EditMode == EditMode::DRAW ? "Stop Draw"
                                                        : "Begin Draw";

            if (ImGui::Button(draw_label.c_str())) {
                m_edit_state.EditMode = m_edit_state.EditMode == EditMode::DRAW
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
            m_layers.emplace_back();
        }
        ImGui::SeparatorText("Select Layer");
        for (int i = 0; i < m_layers.size(); i++) {
            char layer_label[LabelLen + 1];
            snprintf(layer_label, LabelLen, "Layer %d", i);
            if (ImGui::Selectable(layer_label, m_current_layer == i)) {
                m_current_layer = i;
                m_current_object = nullptr;
            }

            char button_label[LabelLen + 1];
            if (m_hidden_layers.find(i) == m_hidden_layers.end()) {
                snprintf(button_label, LabelLen, "Hide %d", i);
                if (ImGui::Button(button_label, ImVec2(60, 20))) {
                    SDL_Log("layer %d should be hidden", i);
                    m_hidden_layers.insert(i);
                }
            } else {
                snprintf(button_label, LabelLen, "Show %d", i);
                if (ImGui::Button(button_label, ImVec2(60, 20))) {
                    m_hidden_layers.erase(i);
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
    if (m_key_map->CheckInputs(EditorAction::PAN_CAMERA_UP)) {
        Renderer::GetInstance()->MoveCameraY(-10.0F);
    }
    if (m_key_map->CheckInputs(EditorAction::PAN_CAMERA_DOWN)) {
        Renderer::GetInstance()->MoveCameraY(10.0F);
    }
    if (m_key_map->CheckInputs(EditorAction::PAN_CAMERA_LEFT)) {
        Renderer::GetInstance()->MoveCameraX(-10.0F);
    }
    if (m_key_map->CheckInputs(EditorAction::PAN_CAMERA_RIGHT)) {
        Renderer::GetInstance()->MoveCameraX(10.0F);
    }

    // Check and handle tool selection / deselection via keybinds
    // the EditorAction param is the result of a satisfied keybind input, with the EditMode param being the tool selection
    CheckForToolSelection(EditorAction::ENTER_DRAW_TOOL, EditMode::DRAW);
    CheckForToolSelection(EditorAction::ENTER_ERASE_TOOL, EditMode::ERASE);
    CheckForToolSelection(EditorAction::ENTER_TILE_SELECT_TOOL, EditMode::TILE_SELECT);
    CheckForToolSelection(EditorAction::ENTER_SELECTION_MOVE_TOOL, EditMode::DRAG_MOVE);
    CheckForToolSelection(EditorAction::ENTER_PAINT_BUCKET_TOOL, EditMode::PAINT_BUCKET);

    // Tool deselection
    if (m_key_map->CheckInputs(EditorAction::EXIT_CURRENT_TOOL)) {
        StopEditing();

        // deselect all
        m_selected_objects.clear();
    }

    // COPY & PASTE COMBOS
    m_key_map->CheckInputs(EditorAction::COPY_SELECTION);
    m_key_map->CheckInputs(EditorAction::PASTE_SELECTION);

    // UNDO & REDO COMBOS
    m_key_map->CheckInputs(EditorAction::UNDO_ACTION);
    m_key_map->CheckInputs(EditorAction::REDO_ACTION);

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
    for (int i = 0; i < m_layers.size(); i++) {
        if (m_hidden_layers.find(i) == m_hidden_layers.end()) {
            for (auto* obj : m_layers[i]) {
                obj->Update(0.0);
                obj->Draw();
                if (m_selected_objects.find(obj) != m_selected_objects.end()) {
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
    if (static_cast<int>(m_cursor->GetCursorType()) != 0) {
        ImGui::SetMouseCursor(ImGuiMouseCursor_None);  // hide default cursor
        std::string const cursor_texture_id = m_cursor->GetTextureId(static_cast<int>(m_edit_state.EditMode));
        SDL_Rect const cursor_rect = m_cursor->UpdateAndGetRect(
            m_cursor_offsets[cursor_texture_id].first, m_cursor_offsets[cursor_texture_id].second);

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
    auto it = m_layers[m_current_layer].end();
    while (it != m_layers[m_current_layer].begin()) {
        --it;
        if (CheckMouseOver(*it)) {
            obj = *it;
            return obj;
        }
    }
    return obj;
}

void Editor::OnMouseClicked(SDL_Event& event) {
    m_mouse_input_origin = GetMouseTilePos();
    m_edit_state.PrevCoords = m_mouse_input_origin;

    switch(m_edit_state.EditMode) {
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
        case EditMode::PAINT_BUCKET:
            HandlePaintBucketAction(event);
            break;
        default:
            throw(std::runtime_error("Unsupported or missing EditMode assigned to m_Editstate"));
    }
}

void Editor::OnMouseMoved(SDL_Event& event) {
    if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
        switch(m_edit_state.EditMode) {
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
            case EditMode::PAINT_BUCKET:
                break;
            default:
                throw(std::runtime_error("Unsupported or missing EditMode assigned to m_Editstate"));
        }
        m_edit_state.PrevCoords = GetMouseTilePos();
        if (m_edit_state.EditMode != EditMode::NONE)
            m_edit_state.IsEditing = true;
    }
}

void Editor::OnMouseUp(SDL_Event& event) {

    switch(m_edit_state.EditMode) {
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
        case EditMode::PAINT_BUCKET:
            HandlePaintBucketAction(event);
            break;
        default:
            throw(std::runtime_error("Unsupported or missing EditMode assigned to m_Editstate"));
    }
    m_edit_state.IsEditing = false;
    m_edit_state.PrevCoords = GetMouseTilePos();
}

void Editor::SnapToGrid(float x, float y, GameObject* obj) {
    float next_x = (static_cast<int>(x + TileSize / 2) / TileSize) * TileSize;
    float next_y = (static_cast<int>(y + TileSize / 2) / TileSize) * TileSize;
    obj->SetX(next_x);
    obj->SetY(next_y);
}

std::pair<float, float> Editor::SnapToGrid(float x, float y) {
    float next_x;
    float next_y;
    next_x = (static_cast<int>(x + TileSize / 2) / TileSize) * TileSize;
    next_y = (static_cast<int>(y + TileSize / 2) / TileSize) * TileSize;

    return {next_x, next_y};
}

std::pair<int, int> Editor::PixelToTilePos(float x, float y) {
    int row;
    int col;
    row = y / TileSize;
    col = x / TileSize;

    return {row, col};
}

TileCoords Editor::PixelToTileCoords(float x, float y) {
    int row, col;
    row = y / TileSize;
    col = x / TileSize;

    return {row, col};
}

std::pair<float, float> Editor::TileCoordsToPixels(TileCoords coords) {
    float y = coords.row * TileSize;
    float x = coords.col * TileSize;
    return {x,y};
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
    if (m_key_map->CheckInputs(editor_action)) {
        m_edit_state.EditMode = m_edit_state.EditMode != edit_mode ? edit_mode : EditMode::NONE;
        m_cursor->SetCursor(m_edit_state.EditMode);
    }
}

// x and y are tile coords
bool Editor::SelectTile(int row, int col) {
    bool selectedOrDeselectedATile = false;

    for (GameObject* obj : m_layers[m_current_layer]) {
        if (obj == nullptr) continue;

        std::pair<int, int> obj_tile_coords = PixelToTilePos(obj->GetX(), obj->GetY());

        if (obj_tile_coords.first == row && obj_tile_coords.second == col) {
            selectedOrDeselectedATile = true;

            if (m_selected_objects.contains(obj)) {
                if (!m_edit_state.IsEditing) {
                    m_selected_objects.erase(obj);                    
                }
            } else { m_selected_objects.insert(obj); }
        }
    }

    return selectedOrDeselectedATile;
}

void Editor::HandleDrawAction() {
    const auto [x,y] = GetMousePixelPos();

    if (m_edit_state.IsEditing && x == m_edit_state.PrevX && y == m_edit_state.PrevY)
        return;

    AddObject(x, y);

    m_edit_state.PrevX = x;
    m_edit_state.PrevY = y;
    m_edit_state.IsEditing = true;

}

void Editor::HandleEraseAction() {
    const auto [x,y] = GetMousePixelPos();

    if (m_edit_state.IsEditing && x == m_edit_state.PrevX && y == m_edit_state.PrevY)
        return;

    // delete all objects on mousedover tile, in current layer.
    std::pair<int, int> tile_coords = PixelToTilePos(x, y);

    // create list of objects to delete on that tile and layer
    std::vector<GameObject*> objects_to_delete;

    for (GameObject* obj : m_layers[m_current_layer]) {
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
    if (!m_selected_objects.empty() && IsMouseOverASelectedTile(mouse_tile_coords) && mouse_moved) {
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
        m_mouse_input_origin = mouse_tile_coords;        
    }

    if (event.button.type == SDL_MOUSEBUTTONUP) {
        bool clickedEmptyTile = false;

        // toggle tile selection
        if (m_mouse_input_origin == mouse_tile_coords)
            clickedEmptyTile = !SelectTile(mouse_tile_coords.row, mouse_tile_coords.col);

        // deslect all
        if (clickedEmptyTile)
            m_selected_objects.clear();

        m_edit_state.IsEditing = false;
    }
}

// tile select
void Editor::HandleTileSelectAction(bool mouse_moved, SDL_Event& event) {
    // update if needed
    if (m_edit_state.EditMode != EditMode::TILE_SELECT)
        UpdateEditMode(EditMode::TILE_SELECT, true);

    TileCoords mouse_tile_coords = GetMouseTilePos();
    if (m_edit_state.PrevCoords == mouse_tile_coords && m_edit_state.IsEditing)
        return;

    // if found nothing deselect all && change edit mode
    bool foundObj = SelectTile(mouse_tile_coords.row, mouse_tile_coords.col);
    if (!foundObj && !mouse_moved) {
        m_selected_objects.clear();
        StopEditing();
    }

    // stop editing on mouse up
    if (event.button.type == SDL_MOUSEBUTTONUP)
        m_edit_state.IsEditing = false;
}

void Editor::HandleDragMoveAction(SDL_Event& event) {
    const auto [x,y] = GetMousePixelPos();
    TileCoords mouse_tile_coords = GetMouseTilePos();

    // update if needed
    if (m_edit_state.EditMode != EditMode::DRAG_MOVE)
        UpdateEditMode(EditMode::DRAG_MOVE, true);

    // finish move on mouse up
    if (event.button.type == SDL_MOUSEBUTTONUP) {
        for (const auto& obj : m_selected_objects) 
            SnapToGrid(obj->GetX(), obj->GetY(), obj);

        StopEditing();
        return;
    }

    // on mouse move, move selection
    if (event.button.type == SDL_MOUSEMOTION) {
        float const dx = event.button.x - InputChecker::GetMouseX();
        float const dy = event.button.y - InputChecker::GetMouseY();
        for (const auto& obj : m_selected_objects)
            MoveObject(obj, dx, dy);
    }

}

void Editor::HandlePaintBucketAction(SDL_Event& event) {
    TileCoords mouse_tile_coords = GetMouseTilePos();

    // on mouse down do nothing

    // on mouse up check validity and paint
    if (event.button.type == SDL_MOUSEBUTTONUP && m_mouse_input_origin == mouse_tile_coords) {
        // search
        std::unordered_set<TileCoords, TileCoords>visited;
        std::queue<TileCoords>tile_queue;
        tile_queue.push(mouse_tile_coords);

        int count=0;

        while (!tile_queue.empty()) {
            TileCoords curr = tile_queue.front();
            tile_queue.pop();

            // paint
            const auto [x,y] = TileCoordsToPixels(curr);
            AddObject(x,y);

            std::vector<TileCoords> neighbors = {{curr.row-1, curr.col}, {curr.row, curr.col+1},
                                                {curr.row+1, curr.col}, {curr.row, curr.col-1}};

            // check neighbors
            for(auto n : neighbors) {
                if (!visited.contains(n)) {
                    visited.insert(n);
                    if (!n.IsInBounds())
                        continue;
                    if (IsTileEmpty(n))
                        tile_queue.push(n);
                }
            }
        }
    }
}

bool Editor::IsTileEmpty(TileCoords coords) {
    for (const auto& obj : m_layers[m_current_layer]) {
        TileCoords obj_coords = PixelToTileCoords(obj->GetX(), obj->GetY());

        if (obj_coords == coords)
            return false;
    }
    return true;
}

bool Editor::IsMouseOverASelectedTile(TileCoords coords) {
    for (const auto& obj : m_selected_objects) {
        TileCoords obj_coords = PixelToTileCoords(obj->GetX(), obj->GetY());
        
        if (obj_coords == coords) 
            return true;
    }
    return false;
}

std::tuple<float,float>Editor::GetMousePixelPos() {
    float const x = ((InputChecker::GetMouseX() + Renderer::GetInstance()->GetCameraX()) / TileSize) * TileSize;
    float const y = ((InputChecker::GetMouseY() + Renderer::GetInstance()->GetCameraY()) / TileSize) * TileSize;

    return {x,y};
}

TileCoords Editor::GetMouseTilePos() {
    // pixel pos
    float const pixelX = ((InputChecker::GetMouseX() + Renderer::GetInstance()->GetCameraX()) / TileSize) * TileSize;
    float const pixelY = ((InputChecker::GetMouseY() + Renderer::GetInstance()->GetCameraY()) / TileSize) * TileSize;

    // conversion
    int row = pixelY / TileSize;
    int col = pixelX / TileSize;

    return {row, col};
}

void Editor::UpdateEditMode(EditMode mode, bool isEditing) {
    m_edit_state.IsEditing = isEditing;
    m_edit_state.EditMode = mode;
    m_cursor->SetCursor(m_edit_state.EditMode);
}

void Editor::StopEditing() {
    m_edit_state.IsEditing = false;
    m_edit_state.EditMode = EditMode::NONE;
    m_cursor->SetCursor(m_edit_state.EditMode);
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

            m_cursor_offsets[id] = {offsetX, offsetY};
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