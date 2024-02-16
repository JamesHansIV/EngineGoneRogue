#include "Application.h"
#include "Engine/Renderer/Renderer.h"

#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/GameObject.h"

#include "Engine/Input/InputChecker.h"

#include "Engine/Timer/Timer.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"

#include "Engine/utils/utils.h"

#include <dirent.h>
#include <unistd.h>

Application* Application::m_instance = nullptr;

const float kDt = 0.01;

Application::Application() : m_ProjectName("test_project"), m_Frame(0) {

    if (SDL_Init(SDL_INIT_VIDEO) != 0 &&
        IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        assert(false);
    }

    m_Window = SDL_CreateWindow("Engine Gone Rogue", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH,
                                SCREEN_HEIGHT, 0);
    if (m_Window == nullptr) {
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        assert(false);
    }

    m_instance = this;

    //TODO: note that the cwd is <projectDir>/build instead of <projectDir>.
    //      Set a working directory path macro to use absolute file paths
    Renderer::GetInstance()->Init();

    if (LOAD_PROJECT) {
        if (!LoadProject()) {
            SDL_Log("Failed to load project");
            assert(false);
        }
    }

    m_IsRunning = true;
}

bool Application::LoadTextures(char* projectPath) {
    tinyxml2::XMLDocument doc;
    std::string textures_path;
    textures_path += projectPath;
    textures_path += "/textures.xml";
    SDL_Log("%s", textures_path.c_str());
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
    int tile_size;
    int rows;
    int cols;

    while (curr_texture != nullptr) {
        type = curr_texture->Attribute("type");
        id = curr_texture->FirstChildElement("ID")->GetText();
        texture_path = curr_texture->FirstChildElement("FilePath")->GetText();
        if (type == "tileMap") {
            SDL_Log("texture filepath: %s", texture_path.c_str());
            tile_size =
                atoi(curr_texture->FirstChildElement("TileSize")->GetText());
            rows = atoi(curr_texture->FirstChildElement("Rows")->GetText());
            cols = atoi(curr_texture->FirstChildElement("Cols")->GetText());
            Renderer::GetInstance()->AddTileMap(id, texture_path, tile_size,
                                                rows, cols);
        } else {
            Renderer::GetInstance()->AddTexture(id, texture_path);
        }

        curr_texture = curr_texture->NextSiblingElement("Texture");
    }
    return true;
}

GameObject* Application::LoadObject(tinyxml2::XMLElement* xmlObj) {
    tinyxml2::XMLElement* texture_id = xmlObj->FirstChildElement("TextureID");
    tinyxml2::XMLElement* object_id = xmlObj->FirstChildElement("ObjectID");
    tinyxml2::XMLElement* src_rect = xmlObj->FirstChildElement("SrcRect");
    tinyxml2::XMLElement* dst_rect = xmlObj->FirstChildElement("DstRect");
    tinyxml2::XMLElement* rotation = xmlObj->FirstChildElement("Rotation");

    std::string const texture_id_val = texture_id->GetText();
    std::string const object_id_val = object_id->GetText();
    SDL_Log("Texture id: %s", texture_id_val.c_str());
    SDL_Log("Object id: %s", object_id_val.c_str());
    TilePos tile_pos;
    tile_pos.row = atoi(src_rect->FirstChildElement("Row")->GetText());
    tile_pos.col = atoi(src_rect->FirstChildElement("Column")->GetText());
    tile_pos.w = atoi(src_rect->FirstChildElement("Width")->GetText());
    tile_pos.h = atoi(src_rect->FirstChildElement("Height")->GetText());

    SDL_Log("Src row: %d", tile_pos.row);
    SDL_Log("Src col: %d", tile_pos.col);
    SDL_Log("Src w: %d", tile_pos.w);
    SDL_Log("Src h: %d", tile_pos.h);

    Rect dst_rect_val;
    dst_rect_val.x = std::stof(dst_rect->FirstChildElement("XPos")->GetText());
    dst_rect_val.y = std::stof(dst_rect->FirstChildElement("YPos")->GetText());
    dst_rect_val.w = atoi(dst_rect->FirstChildElement("Width")->GetText());
    dst_rect_val.h = atoi(dst_rect->FirstChildElement("Height")->GetText());

    SDL_Log("Dst x: %f", dst_rect_val.x);
    SDL_Log("Dst y: %f", dst_rect_val.y);
    SDL_Log("dst w: %d", dst_rect_val.w);
    SDL_Log("dst h: %d", dst_rect_val.h);

    GameObject* obj = nullptr;

    float const angle = std::stof(rotation->GetText());

    Properties props(texture_id_val, tile_pos, dst_rect_val, angle,
                     object_id_val);

    obj = new GameObject(props);

    // tinyxml2::XMLElement* collider = xmlObj->FirstChildElement("CollisionBox");
    // SDL_Log("CollisionBox exists: %d", collider != nullptr);
    // if (collider != nullptr) {
    //     obj->SetCollider(new CollisionBox());
    //     obj->GetCollider()->Set(
    //         atoi(collider->FirstChildElement("XPos")->GetText()),
    //         atoi(collider->FirstChildElement("YPos")->GetText()),
    //         atoi(collider->FirstChildElement("Width")->GetText()),
    //         atoi(collider->FirstChildElement("Height")->GetText())
    //     );
    //     SDL_Log("collider x: %d", obj->GetCollider()->Get().x);
    //     SDL_Log("collider y: %d", obj->GetCollider()->Get().y);
    //     SDL_Log("collider w: %d", obj->GetCollider()->Get().w);
    //     SDL_Log("collider h: %d", obj->GetCollider()->Get().h);
    // }

    tinyxml2::XMLElement* animation = xmlObj->FirstChildElement("Animation");

    if (animation != nullptr) {
        obj->SetAnimation(new Animation());
        TilePos const tile_pos = {
            atoi(animation->FirstChildElement("Row")->GetText()),
            atoi(animation->FirstChildElement("Col")->GetText()),
            atoi(animation->FirstChildElement("Width")->GetText()),
            atoi(animation->FirstChildElement("Height")->GetText()),
        };
        obj->GetAnimation()->SetProps(
            {animation->FirstChildElement("TextureID")->GetText(), tile_pos,
             atoi(animation->FirstChildElement("FrameCount")->GetText()),
             atoi(animation->FirstChildElement("Speed")->GetText())});
        SDL_Log("Animation row: %d", obj->GetAnimation()->GetSpriteRow());
        SDL_Log("Animation col: %d", obj->GetAnimation()->GetSpriteCol());
        SDL_Log("Animation frameCount: %d",
                obj->GetAnimation()->GetFrameCount());
        SDL_Log("Animation speed: %d",
                obj->GetAnimation()->GetAnimationSpeed());
    }

    return obj;
}

Collider* Application::LoadCollider(tinyxml2::XMLElement* xmlObj,
                                    GameObject* obj) {
    auto* collider = new Collider(obj);

    if (collider == nullptr) {
        SDL_Log("failed to create collider");
        assert(false);
    };
    SDL_Log("Created collider");

    tinyxml2::XMLElement* collision_box =
        xmlObj->FirstChildElement("CollisionBox");

    collider->GetCollisionBox().Set(
        atoi(collision_box->FirstChildElement("XPos")->GetText()),
        atoi(collision_box->FirstChildElement("YPos")->GetText()),
        atoi(collision_box->FirstChildElement("Width")->GetText()),
        atoi(collision_box->FirstChildElement("Height")->GetText()));

    delete obj;
    return collider;
}

bool Application::LoadObjects(const std::string& roomPath,
                              const std::string& roomID) {
    tinyxml2::XMLDocument doc;

    SDL_Log("%s", roomPath.c_str());
    tinyxml2::XMLError const error = doc.LoadFile(roomPath.c_str());
    if (error != tinyxml2::XML_SUCCESS) {
        SDL_LogError(0, "Could not load objects");
        return false;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("Root");
    tinyxml2::XMLElement* curr_object = root->FirstChildElement("Object");
    tinyxml2::XMLElement* types = nullptr;

    if (root == nullptr) {
        SDL_Log("Root element does not exist for room %s", roomID.c_str());
        assert(false);
    }
    if (curr_object == nullptr) {
        SDL_Log("No objects exist in room %s", roomID.c_str());
        return true;
    }

    GameObject* created_obj = nullptr;
    while (curr_object != nullptr) {
        types = curr_object->FirstChildElement("Types");
        if (types == nullptr) {
            SDL_Log("Object does not contain types element");
            assert(false);
        }

        created_obj = LoadObject(curr_object);
        SDL_Log("loaded object: %d", static_cast<int>(created_obj != nullptr));
        if (created_obj == nullptr) {
            return false;
        }

        if (types->FirstChildElement("Collider") != nullptr) {
            created_obj = LoadCollider(curr_object, created_obj);
            SDL_Log("loaded collider: %d aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                    static_cast<int>(dynamic_cast<Collider*>(created_obj) ==
                                     nullptr));
        }

        if (created_obj == nullptr) {
            SDL_Log("maybe collider load failed?");
            assert(false);
        };

        m_Rooms[roomID].push_back(created_obj);

        curr_object = curr_object->NextSiblingElement("Object");
    }
    return true;
}

bool Application::LoadRooms(const char* projectPath) {
    struct dirent* entry;
    DIR* dp;

    std::string rooms_path = projectPath;
    rooms_path += "/rooms";
    dp = opendir(rooms_path.c_str());
    if (dp == nullptr) {
        perror("Rooms path does not exist");
        return false;
    }

    std::string room_path = rooms_path;
    std::string room_id;
    int const index = 0;
    while ((entry = readdir(dp)) != nullptr) {
        if (strcmp(entry->d_name, ".") != 0 &&
            strcmp(entry->d_name, "..") != 0) {
            std::string const file_name = entry->d_name;
            room_path += "/";
            room_path += entry->d_name;
            room_id = file_name.substr(0, file_name.rfind('.'));
            m_Rooms[room_id] = std::vector<GameObject*>();
            if (!LoadObjects(room_path, room_id)) {
                return false;
            }
            room_path = rooms_path;
        }
    }

    closedir(dp);
    return true;
}

bool Application::LoadProject() {
    char project_path[FILEPATH_LEN + 1];
    snprintf(project_path, FILEPATH_LEN, "../assets/projects/%s",
             m_ProjectName.c_str());
    if (!LoadTextures(project_path)) {
        return false;
    }
    SDL_Log("Textures are fine");
    SDL_Log("%s", project_path);
    return LoadRooms(project_path);
}

void Application::Events() {
    SDL_Event event;
    SDL_Log("Event");
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            SDL_Log("Event type: %d", event.type);
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
                break;
            case SDL_MOUSEBUTTONUP:
                InputChecker::SetMouseButtonPressed(event.button.button, false);
                break;
            case SDL_MOUSEMOTION:
                InputChecker::UpdateMousePosition(event.motion.x,
                                                  event.motion.y);
                break;
            case SDL_MOUSEWHEEL:
                InputChecker::SetMouseWheelDirection(event.wheel.y);
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED) {
                    m_has_focus = true;
                    m_is_paused = false;
                    m_LastTick = SDL_GetTicks();
                }
                if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
                    SDL_Log("Lost focus");
                    m_has_focus = false;
                    m_is_paused = true;
                }
                break;
        }
    }
}

void Application::Run() {
    timer.Start();
    float accumulator = 0.0;
    while (m_IsRunning) {
        // SDL_Log("Time: %d", timer.GetTicks() / 1000);
        //SDL_Log("Current time: %d", timer.GetCurrentTime());
        Events();
        if (!m_is_paused) {
            Uint32 const new_time = timer.GetTicks();
            auto frame_time =
                static_cast<float>(new_time - timer.GetCurrentTime());

            if (frame_time > 0.25) {
                frame_time = 0.25;
            }

            timer.SetCurrentTime(new_time);

            accumulator += frame_time;

            while (accumulator >= kDt) {
                m_Frame++;
                Update(kDt);
                accumulator -= kDt;
            }
        }
        Render();
    }
    Clean();
}

bool Application::Clean() {
    Renderer::GetInstance()->Destroy();
    delete Renderer::GetInstance();

    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    return true;
}

void Application::Quit() {
    m_IsRunning = false;
}
