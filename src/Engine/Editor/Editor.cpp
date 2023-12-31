#define IMGUI_IMPL_API

#include "Editor.h"
#include "Engine/Renderer/Renderer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"
#include "imgui.h"

#include "Engine/Input/InputChecker.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/Player.h"
#include "Engine/Objects/Projectile.h"

#include <dirent.h>
#include <sys/stat.h>
#include <tinyxml2.h>
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
    for (auto& m_layer : m_Layers) {
        for (auto* obj : m_layer) {
            delete obj;
        }
    }
    m_Layers.clear();
}

void WriteCollider(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                   Collider* obj) {
    tinyxml2::XMLElement* collider = doc.NewElement("CollisionBox");
    tinyxml2::XMLElement* collider_x = doc.NewElement("XPos");
    tinyxml2::XMLElement* collider_y = doc.NewElement("YPos");
    tinyxml2::XMLElement* collider_w = doc.NewElement("Width");
    tinyxml2::XMLElement* collider_h = doc.NewElement("Height");
    collider_x->SetText(
        std::to_string(obj->GetCollisionBox().GetRect().x).c_str());
    collider_y->SetText(
        std::to_string(obj->GetCollisionBox().GetRect().y).c_str());
    collider_w->SetText(
        std::to_string(obj->GetCollisionBox().GetRect().w).c_str());
    collider_h->SetText(
        std::to_string(obj->GetCollisionBox().GetRect().h).c_str());

    collider->InsertEndChild(collider_x);
    collider->InsertEndChild(collider_y);
    collider->InsertEndChild(collider_w);
    collider->InsertEndChild(collider_h);

    xmlObj->InsertEndChild(collider);
}

void WriteBaseObject(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                     GameObject* obj) {
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

    if (obj->GetAnimation() != nullptr) {
        tinyxml2::XMLElement* animation = doc.NewElement("Animation");
        tinyxml2::XMLElement* animation_tex_id = doc.NewElement("TextureID");
        tinyxml2::XMLElement* animation_row = doc.NewElement("Row");
        tinyxml2::XMLElement* animation_col = doc.NewElement("Col");
        tinyxml2::XMLElement* animation_w = doc.NewElement("Width");
        tinyxml2::XMLElement* animation_h = doc.NewElement("Height");
        tinyxml2::XMLElement* frame_count = doc.NewElement("FrameCount");
        tinyxml2::XMLElement* speed = doc.NewElement("Speed");
        animation_tex_id->SetText(obj->GetAnimation()->GetTextureID().c_str());
        animation_row->SetText(
            std::to_string(obj->GetAnimation()->GetSpriteRow()).c_str());
        animation_col->SetText(
            std::to_string(obj->GetAnimation()->GetSpriteCol()).c_str());
        animation_w->SetText(
            std::to_string(obj->GetAnimation()->GetSpriteWidth()).c_str());
        animation_h->SetText(
            std::to_string(obj->GetAnimation()->GetSpriteHeight()).c_str());
        frame_count->SetText(
            std::to_string(obj->GetAnimation()->GetFrameCount()).c_str());
        speed->SetText(
            std::to_string(obj->GetAnimation()->GetAnimationSpeed()).c_str());

        animation->InsertEndChild(animation_tex_id);
        animation->InsertEndChild(animation_row);
        animation->InsertEndChild(animation_col);
        animation->InsertEndChild(animation_w);
        animation->InsertEndChild(animation_h);
        animation->InsertEndChild(frame_count);
        animation->InsertEndChild(speed);

        xmlObj->InsertEndChild(animation);
    }
}

void Editor::SaveRoom(const char* roomName) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("Root");
    doc.InsertFirstChild(root);

    tinyxml2::XMLElement* curr_xml_object;
    tinyxml2::XMLElement* types;
    tinyxml2::XMLElement* type;

    for (const auto& layer : m_Layers) {
        for (auto* obj : layer) {
            curr_xml_object = doc.NewElement("Object");
            types = doc.NewElement("Types");
            curr_xml_object->InsertEndChild(types);

            WriteBaseObject(doc, curr_xml_object, obj);

            if (auto* collider = dynamic_cast<Collider*>(obj)) {
                WriteCollider(doc, curr_xml_object, collider);
                type = doc.NewElement("Collider");
                type->SetText("True");
                types->InsertEndChild(type);
            }
            root->InsertEndChild(curr_xml_object);
        }
    }

    char file_path[FILEPATH_LEN + 1];
    snprintf(file_path, FILEPATH_LEN, "../assets/projects/%s/rooms/%s.xml",
             m_ProjectName.c_str(), roomName);
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

        if (ImGui::Button("Save Project", ImVec2(150, 20))) {
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
            m_DrawState.EditMode == EditMode::SELECT ? "Stop Select"
                                                     : "Begin Select";
        if (ImGui::Button(select_label.c_str())) {
            m_CurrentObject = nullptr;
            m_DrawState.EditMode = m_DrawState.EditMode == EditMode::SELECT
                                       ? EditMode::NONE
                                       : EditMode::SELECT;
        }

        if (ImGui::Button("Deselect")) {
            m_SelectedObjects.clear();
            m_DrawState.EditMode = EditMode::NONE;
        }

        std::string const erase_label = m_DrawState.EditMode == EditMode::ERASE
                                            ? "Stop Erase"
                                            : "Begin Erase";
        if (ImGui::Button(erase_label.c_str())) {
            m_DrawState.EditMode = m_DrawState.EditMode == EditMode::ERASE
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
                m_DrawState.EditMode == EditMode::DRAW ? "Stop Draw"
                                                       : "Begin Draw";

            if (ImGui::Button(draw_label.c_str())) {
                m_DrawState.EditMode = m_DrawState.EditMode == EditMode::DRAW
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
    if (InputChecker::IsKeyPressed(SDLK_UP)) {
        Renderer::GetInstance()->MoveCameraY(-10.0F);
    }
    if (InputChecker::IsKeyPressed(SDLK_DOWN)) {
        Renderer::GetInstance()->MoveCameraY(10.0F);
    }
    if (InputChecker::IsKeyPressed(SDLK_LEFT)) {
        Renderer::GetInstance()->MoveCameraX(-10.0F);
    }
    if (InputChecker::IsKeyPressed(SDLK_RIGHT)) {
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

void Editor::OnMouseClicked(SDL_Event& /*event*/) {
    if (m_DrawState.EditMode != EditMode::NONE) {
        m_DrawState.IsEditing = true;

        float const x = ((InputChecker::GetMouseX() +
                          Renderer::GetInstance()->GetCameraX()) /
                         TILE_SIZE) *
                        TILE_SIZE;
        float const y = ((InputChecker::GetMouseY() +
                          Renderer::GetInstance()->GetCameraY()) /
                         TILE_SIZE) *
                        TILE_SIZE;

        if (m_DrawState.EditMode == EditMode::DRAW) {
            AddObject(x, y);
        } else if (m_DrawState.EditMode == EditMode::ERASE) {
            GameObject* obj = GetObjectUnderMouse();
            if (obj != nullptr) {
                DeleteObject(obj);
            }
        } else {
            GameObject* obj = GetObjectUnderMouse();

            if (obj != nullptr) {
                m_SelectedObjects.insert(obj);
            }
            for (const auto& obj : m_SelectedObjects) {
                SDL_Log("%s", obj->GetID().c_str());
            }
            SDL_Log("\n");
        }
        m_DrawState.PrevX = x;
        m_DrawState.PrevY = y;

    } else {
        GameObject* obj = GetObjectUnderMouse();
        if (obj != nullptr) {
            auto it = std::find(m_Layers[m_CurrentLayer].begin(),
                                m_Layers[m_CurrentLayer].end(), obj);
            m_Layers[m_CurrentLayer].erase(it);
            m_Layers[m_CurrentLayer].push_back(obj);
            m_CurrentObject = obj;
        }
    }
}

void Editor::OnMouseMoved(SDL_Event& event) {
    if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {

        if (m_DrawState.IsEditing) {
            float const x = ((InputChecker::GetMouseX() +
                              Renderer::GetInstance()->GetCameraX()) /
                             TILE_SIZE) *
                            TILE_SIZE;
            float const y = ((InputChecker::GetMouseY() +
                              Renderer::GetInstance()->GetCameraY()) /
                             TILE_SIZE) *
                            TILE_SIZE;

            if ((x != m_DrawState.PrevX || y != m_DrawState.PrevY)) {
                if (m_DrawState.EditMode == EditMode::DRAW) {
                    AddObject(x, y);

                } else if (m_DrawState.EditMode == EditMode::ERASE) {
                    GameObject* obj = GetObjectUnderMouse();

                    if (obj != nullptr) {
                        DeleteObject(obj);
                    }
                } else {
                    GameObject* obj = GetObjectUnderMouse();

                    if (obj != nullptr) {
                        m_SelectedObjects.insert(obj);
                    }
                }
                m_DrawState.PrevX = x;
                m_DrawState.PrevY = y;
            }
        } else if (!m_SelectedObjects.empty()) {
            bool mouse_over_any = false;
            for (const auto& obj : m_SelectedObjects) {
                if (CheckMouseOver(obj)) {
                    mouse_over_any = true;
                }
            }
            if (mouse_over_any) {
                float const dx = event.button.x - InputChecker::GetMouseX();
                float const dy = event.button.y - InputChecker::GetMouseY();

                for (const auto& obj : m_SelectedObjects) {
                    MoveObject(obj, dx, dy);
                }
            }

        } else if (m_CurrentObject != nullptr &&
                   CheckMouseOver(m_CurrentObject)) {
            float const dx = event.button.x - InputChecker::GetMouseX();
            float const dy = event.button.y - InputChecker::GetMouseY();
            MoveObject(m_CurrentObject, dx, dy);
        }
    }
}

void Editor::OnMouseUp(SDL_Event& /*event*/) {
    if (InputChecker::IsMouseButtonPressed(SDL_BUTTON_LEFT)) {
        if (m_DrawState.IsEditing) {
            m_DrawState.IsEditing = false;

        } else if (!m_SelectedObjects.empty()) {
            if (m_ObjectInfo.SnapToGrid) {
                for (const auto& obj : m_SelectedObjects) {
                    std::pair<float, float> const coords =
                        SnapToGrid(obj->GetX(), obj->GetY());
                    obj->SetX(coords.first);
                    obj->SetY(coords.second);
                }
            }

        } else if (m_CurrentObject != nullptr &&
                   CheckMouseOver(m_CurrentObject)) {

            if (m_ObjectInfo.SnapToGrid) {
                std::pair<float, float> const coords = SnapToGrid(
                    m_CurrentObject->GetX(), m_CurrentObject->GetY());
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

    return {next_x, next_y};
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

#if EDITOR == 1
Application* CreateApplication() {
    return new Editor();
}
#endif
