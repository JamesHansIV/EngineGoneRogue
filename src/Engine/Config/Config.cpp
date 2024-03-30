#include "Config.h"
#include <cstddef>
#include <fstream>
#include <typeinfo>
#include "Engine/Objects/Characters/Charger.h"
#include "Engine/Objects/Characters/Dog.h"
#include "Engine/Objects/Characters/Goblin.h"
#include "Engine/Objects/Characters/HelixEnemy.h"
#include "Engine/Objects/Characters/Kamikaze.h"
#include "Engine/Objects/Characters/Mage.h"
#include "Engine/Objects/Characters/Player.h"
#include "Engine/Objects/Characters/RingShotEnemy.h"
#include "Engine/Objects/Characters/Skeleton.h"
#include "Engine/Objects/Characters/Slime.h"
#include "Engine/Objects/Collider.h"
#include "Engine/Objects/Environment/Entrance.h"
#include "Engine/Objects/Trap.h"
#include "Engine/Renderer/Renderer.h"

void WriteColliderInfo(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                       Collider* obj) {
    tinyxml2::XMLElement* collider = doc.NewElement("CollisionBox");

    const Rect rect = obj->GetCollisionBox().GetRect();

    collider->SetAttribute("x", rect.x);
    collider->SetAttribute("y", rect.y);
    collider->SetAttribute("w", rect.w);
    collider->SetAttribute("h", rect.h);

    xmlObj->InsertEndChild(collider);
}

void WriteAnimationInfo(tinyxml2::XMLDocument& doc,
                        tinyxml2::XMLElement* xmlObj, GameObject* obj) {

    Animation* animation = obj->GetAnimation();

    tinyxml2::XMLElement* animation_element = doc.NewElement("Animation");

    tinyxml2::XMLElement* curr = nullptr;
    tinyxml2::XMLElement* tile = nullptr;

    for (auto& info : animation->GetAnimationInfo()) {
        auto second = info.second;
        curr = doc.NewElement("AnimationInfo");

        curr->SetAttribute("id", info.first.c_str());
        curr->SetAttribute("texture_id", second.TextureID.c_str());
        curr->SetAttribute("frame_count", second.FrameCount);
        curr->SetAttribute("speed", second.AnimationSpeed);
        curr->SetAttribute("flip", second.Flip);
        curr->SetAttribute("loop", second.Loop);
        curr->SetAttribute("key_frames_start", second.KeyFramesStart);
        curr->SetAttribute("key_frames_end", second.KeyFramesEnd);

        tile = doc.NewElement("Tile");
        tile->SetAttribute("row", second.Tile.row);
        tile->SetAttribute("col", second.Tile.col);
        tile->SetAttribute("w", second.Tile.w);
        tile->SetAttribute("h", second.Tile.h);

        curr->InsertEndChild(tile);
        animation_element->InsertEndChild(curr);
    }

    xmlObj->InsertEndChild(animation_element);
}

void WriteBaseObjectInfo(tinyxml2::XMLDocument& doc,
                         tinyxml2::XMLElement* xmlObj, GameObject* obj) {
    xmlObj->SetAttribute("texture_id", obj->GetTextureID().c_str());
    xmlObj->SetAttribute("object_id", obj->GetID().c_str());
    xmlObj->SetAttribute("rotation", obj->GetRotation());

    tinyxml2::XMLElement* src_rect = doc.NewElement("SrcRect");
    tinyxml2::XMLElement* dst_rect = doc.NewElement("DstRect");

    const TilePos tile_pos = obj->GetTilePos();

    src_rect->SetAttribute("row", tile_pos.row);
    src_rect->SetAttribute("col", tile_pos.col);
    src_rect->SetAttribute("w", tile_pos.w);
    src_rect->SetAttribute("h", tile_pos.h);

    const Rect rect = obj->GetDstRect();

    dst_rect->SetAttribute("x", rect.x);
    dst_rect->SetAttribute("y", rect.y);
    dst_rect->SetAttribute("w", rect.w);
    dst_rect->SetAttribute("h", rect.h);

    xmlObj->InsertEndChild(src_rect);
    xmlObj->InsertEndChild(dst_rect);
}

void WriteStatsInfo(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                    EnemyStats stats) {
    tinyxml2::XMLElement* stats_element = doc.NewElement("EnemyStats");

    stats_element->SetAttribute("health", stats.health);
    stats_element->SetAttribute("damage", stats.damage);
    stats_element->SetAttribute("speed", stats.speed);
    stats_element->SetAttribute("range", stats.range);
    stats_element->SetAttribute("perception_width", stats.perceptionWidth);
    stats_element->SetAttribute("perception_height", stats.perceptionHeight);
    stats_element->SetAttribute("xp", stats.xpGiven);

    xmlObj->InsertEndChild(stats_element);
}

void WriteStatsInfo(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
                    RangedEnemyStats stats) {
    tinyxml2::XMLElement* stats_element = doc.NewElement("RangedEnemyStats");

    stats_element->SetAttribute("health", stats.health);
    stats_element->SetAttribute("damage", stats.damage);
    stats_element->SetAttribute("speed", stats.speed);
    stats_element->SetAttribute("range", stats.range);
    stats_element->SetAttribute("perception_width", stats.perceptionWidth);
    stats_element->SetAttribute("perception_height", stats.perceptionHeight);
    stats_element->SetAttribute("xp", stats.xpGiven);
    stats_element->SetAttribute("fire_interval", stats.fireInterval);
    stats_element->SetAttribute("spread", stats.spread);
    stats_element->SetAttribute("spread_count", stats.spreadCount);

    xmlObj->InsertEndChild(stats_element);
}

int SaveObjects(const char* filepath, const std::vector<GameObject*>& objects) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("Root");
    doc.InsertFirstChild(root);

    tinyxml2::XMLElement* curr_xml_object;
    tinyxml2::XMLElement* types;
    tinyxml2::XMLElement* type;

    std::cout << "Saving " << objects.size() << " objects\n";

    int num_enemies = 0;
    for (auto* obj : objects) {
        // if (obj->GetObjectType() == ObjectType::Enemy) {
        //     num_enemies++;
        //     continue;
        // }
        curr_xml_object = doc.NewElement("Object");
        types = doc.NewElement("Types");
        curr_xml_object->InsertEndChild(types);

        WriteBaseObjectInfo(doc, curr_xml_object, obj);

        if (obj->GetAnimation() != nullptr) {
            WriteAnimationInfo(doc, curr_xml_object, obj);
        }

        if (auto* collider = dynamic_cast<Collider*>(obj)) {
            WriteColliderInfo(doc, curr_xml_object, collider);
            types->SetAttribute("collider", "1");
        }

        // if (obj->GetObjectType() == ObjectType::Enemy) {
        //     WriteStatsInfo(doc, curr_xml_object,
        // }

        // type handling
        const std::type_info& obj_type = typeid(*obj);
        std::cout << "obj type: " << obj_type.name() << std::endl;

        if (strcmp(obj_type.name(), "5Slime") == 0) {
            types->SetAttribute("slime", "1");
            WriteStatsInfo(doc, curr_xml_object,
                           dynamic_cast<Enemy*>(obj)->GetEnemyStats());
        }
        if (strcmp(obj_type.name(), "13RingShotEnemy") == 0) {
            types->SetAttribute("ranged_enemy", "1");
            types->SetAttribute("ring_shot_enemy", "1");
            WriteStatsInfo(
                doc, curr_xml_object,
                dynamic_cast<RangedEnemy*>(obj)->GetRangedEnemyStats());
        }
        if (strcmp(obj_type.name(), "3Dog") == 0) {
            types->SetAttribute("ranged_enemy", "1");
            types->SetAttribute("dog", "1");
            WriteStatsInfo(
                doc, curr_xml_object,
                dynamic_cast<RangedEnemy*>(obj)->GetRangedEnemyStats());
        }
        if (strcmp(obj_type.name(), "10HelixEnemy") == 0) {
            types->SetAttribute("ranged_enemy", "1");
            types->SetAttribute("helix_enemy", "1");
            WriteStatsInfo(
                doc, curr_xml_object,
                dynamic_cast<RangedEnemy*>(obj)->GetRangedEnemyStats());
        }
        if (strcmp(obj_type.name(), "6Goblin") == 0) {
            types->SetAttribute("ranged_enemy", "1");
            types->SetAttribute("goblin", "1");
            WriteStatsInfo(
                doc, curr_xml_object,
                dynamic_cast<RangedEnemy*>(obj)->GetRangedEnemyStats());
        }
        if (strcmp(obj_type.name(), "8Skeleton") == 0) {
            types->SetAttribute("ranged_enemy", "1");
            types->SetAttribute("skeleton", "1");
            WriteStatsInfo(
                doc, curr_xml_object,
                dynamic_cast<RangedEnemy*>(obj)->GetRangedEnemyStats());
        }
        if (strcmp(obj_type.name(), "4Mage") == 0) {
            types->SetAttribute("ranged_enemy", "1");
            types->SetAttribute("mage", "1");
            WriteStatsInfo(
                doc, curr_xml_object,
                dynamic_cast<RangedEnemy*>(obj)->GetRangedEnemyStats());
        }
        root->InsertEndChild(curr_xml_object);
    }

    std::cout << "num enemies " << num_enemies << std::endl;

    return doc.SaveFile(filepath);
}

int SaveTextureMap(const char* filepath,
                   const std::map<std::string, Texture*>& textureMap) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("Root");
    doc.InsertFirstChild(root);

    tinyxml2::XMLElement* texture;
    tinyxml2::XMLElement* id;
    tinyxml2::XMLElement* file_path;
    tinyxml2::XMLElement* type;

    tinyxml2::XMLElement* tile_size;
    tinyxml2::XMLElement* rows;
    tinyxml2::XMLElement* cols;

    for (const auto& it : textureMap) {
        texture = doc.NewElement("Texture");
        type = doc.NewElement("Type");
        id = doc.NewElement("ID");
        file_path = doc.NewElement("FilePath");

        auto* tile_map = dynamic_cast<TileMap*>(it.second);
        if (tile_map != nullptr) {
            SDL_Log("Saving tilemap");
            texture->SetAttribute("type", "tileMap");
            tile_size = doc.NewElement("TileSize");
            rows = doc.NewElement("Rows");
            cols = doc.NewElement("Cols");

            tile_size->SetText(tile_map->GetTileSize());
            rows->SetText(tile_map->GetRows());
            cols->SetText(tile_map->GetCols());

            texture->InsertEndChild(tile_size);
            texture->InsertEndChild(rows);
            texture->InsertEndChild(cols);
        } else {
            texture->SetAttribute("type", "base");
        }

        id->SetText(it.first.c_str());
        file_path->SetText(it.second->GetFilePath().c_str());

        texture->InsertEndChild(file_path);
        texture->InsertEndChild(id);

        root->InsertEndChild(texture);
    }

    int const success = doc.SaveFile(filepath);
    SDL_Log("Saving textures a success: %d", success);
    return success;
}

void AddAnimation(tinyxml2::XMLElement* xmlObj, GameObject* obj) {
    tinyxml2::XMLElement* animation = xmlObj->FirstChildElement("Animation");

    if (animation == nullptr) {
        return;
    }

    obj->SetAnimation(new Animation());

    tinyxml2::XMLElement* curr = animation->FirstChildElement("AnimationInfo");
    tinyxml2::XMLElement* tile;

    while (curr != nullptr) {
        tile = curr->FirstChildElement("Tile");

        TilePos const tile_pos = {
            atoi(tile->Attribute("row")),
            atoi(tile->Attribute("col")),
            atoi(tile->Attribute("w")),
            atoi(tile->Attribute("h")),
        };

        AnimationInfo info = {curr->Attribute("texture_id"), tile_pos,
                              atoi(curr->Attribute("frame_count")),
                              atoi(curr->Attribute("speed"))};

        SDL_Log("info loop: %s", (curr->Attribute("texture_id")));

        if (curr->Attribute("flip") != nullptr) {
            info.Flip =
                static_cast<SDL_RendererFlip>(atoi(curr->Attribute("flip")));
        }

        SDL_Log("checked flip attribute");

        info.Loop = curr->Attribute("loop") != nullptr &&
                    strcmp(curr->Attribute("loop"), "1") == 0;
        SDL_Log("info loop: %d", static_cast<int>(info.Loop));

        SDL_Log("checked loop attribute");

        if (curr->Attribute("key_frames_start") != nullptr &&
            curr->Attribute("key_frames_end") != nullptr) {
            info.KeyFramesStart = atoi(curr->Attribute("key_frames_start"));
            info.KeyFramesEnd = atoi(curr->Attribute("key_frames_end"));
        }

        SDL_Log("checked key frames attributes");

        obj->GetAnimation()->AddAnimation(curr->Attribute("id"), info);
        SDL_Log("added animation for '%s'", curr->Attribute("id"));

        obj->GetAnimation()->SelectAnimation(curr->Attribute("id"));

        curr = curr->NextSiblingElement("AnimationInfo");

        SDL_Log("Animation texture id: %s",
                obj->GetAnimation()->GetAnimationID().c_str());
        SDL_Log("Animation row: %d", obj->GetAnimation()->GetSpriteRow());
        SDL_Log("Animation col: %d", obj->GetAnimation()->GetSpriteCol());
        SDL_Log("Animation frameCount: %d",
                obj->GetAnimation()->GetFrameCount());
        SDL_Log("Animation speed: %d",
                obj->GetAnimation()->GetAnimationSpeed());
    }
}

GameObject* LoadObject(tinyxml2::XMLElement* xmlObj) {
    tinyxml2::XMLElement* src_rect = xmlObj->FirstChildElement("SrcRect");
    tinyxml2::XMLElement* dst_rect = xmlObj->FirstChildElement("DstRect");

    std::string const texture_id_val = xmlObj->Attribute("texture_id");
    std::string const object_id_val = xmlObj->Attribute("object_id");
    float const rotation = std::stof(xmlObj->Attribute("rotation"));

    SDL_Log("Texture id: %s", texture_id_val.c_str());
    SDL_Log("Object id: %s", object_id_val.c_str());
    TilePos tile_pos;
    tile_pos.row = atoi(src_rect->Attribute("row"));
    tile_pos.col = atoi(src_rect->Attribute("col"));
    tile_pos.w = atoi(src_rect->Attribute("w"));
    tile_pos.h = atoi(src_rect->Attribute("h"));

    SDL_Log("Src row: %d", tile_pos.row);
    SDL_Log("Src col: %d", tile_pos.col);
    SDL_Log("Src w: %d", tile_pos.w);
    SDL_Log("Src h: %d", tile_pos.h);

    Rect rect;
    rect.x = std::stof(dst_rect->Attribute("x"));
    rect.y = std::stof(dst_rect->Attribute("y"));
    rect.w = atoi(dst_rect->Attribute("w"));
    rect.h = atoi(dst_rect->Attribute("h"));

    SDL_Log("Dst x: %f", rect.x);
    SDL_Log("Dst y: %f", rect.y);
    SDL_Log("dst w: %d", rect.w);
    SDL_Log("dst h: %d", rect.h);

    GameObject* obj = nullptr;

    Properties props(texture_id_val, tile_pos, rect, rotation, object_id_val);

    obj = new GameObject(props);

    AddAnimation(xmlObj, obj);

    return obj;
}

Collider* LoadCollider(tinyxml2::XMLElement* xmlObj, GameObject* obj) {
    auto* collider = new Collider(obj);

    if (collider == nullptr) {
        SDL_Log("failed to create collider");
        assert(false);
    };
    SDL_Log("Created collider");

    tinyxml2::XMLElement* collision_box =
        xmlObj->FirstChildElement("CollisionBox");

    if (collision_box == nullptr) {
        SDL_Log("Collider %s does not have a collision box set",
                obj->GetID().c_str());
        collider->GetCollisionBox().Set(
            obj->GetDstRect().x, obj->GetDstRect().y, obj->GetDstRect().w,
            obj->GetDstRect().h);
    } else {
        collider->GetCollisionBox().Set(atoi(collision_box->Attribute("x")),
                                        atoi(collision_box->Attribute("y")),
                                        atoi(collision_box->Attribute("w")),
                                        atoi(collision_box->Attribute("h")));
    }

    delete obj;
    return collider;
}

EnemyStats GetEnemyStats(tinyxml2::XMLElement* element) {

    EnemyStats stats;
    stats.health = atoi(element->Attribute("health"));
    stats.damage = atoi(element->Attribute("damage"));
    stats.speed = atoi(element->Attribute("speed"));
    stats.range = atof(element->Attribute("range"));
    stats.perceptionWidth = atoi(element->Attribute("perception_width"));
    stats.perceptionHeight = atoi(element->Attribute("perception_height"));
    stats.xpGiven = atoi(element->Attribute("xp"));

    return stats;
}

RangedEnemyStats GetRangedEnemyStats(tinyxml2::XMLElement* element,
                                     EnemyStats stats) {
    SDL_Log("getting ranged enemy stats");
    RangedEnemyStats new_stats;
    new_stats.health = stats.health;
    new_stats.damage = stats.damage;
    new_stats.speed = stats.speed;
    new_stats.range = stats.range;
    new_stats.perceptionWidth = stats.perceptionWidth;
    new_stats.xpGiven = stats.xpGiven;
    new_stats.fireInterval = atoi(element->Attribute("fire_interval"));
    //Spread is saved as a fraction of PI
    if (element->Attribute("spread") != nullptr &&
        element->Attribute("spread_count") != nullptr) {
        new_stats.spread = M_PI * atof(element->Attribute("spread"));
        new_stats.spreadCount = M_PI * atoi(element->Attribute("spread_count"));
    }
    return new_stats;
}

GameObject* BuildRangedEnemy(tinyxml2::XMLElement* types,
                             tinyxml2::XMLElement* xmlObj, GameObject* obj) {

    GameObject* new_obj = nullptr;
    EnemyStats const stats =
        GetEnemyStats(xmlObj->FirstChildElement("RangedEnemyStats"));
    RangedEnemyStats const ranged_stats = GetRangedEnemyStats(
        xmlObj->FirstChildElement("RangedEnemyStats"), stats);

    if (types->Attribute("ring_shot_enemy") != nullptr) {
        SDL_Log("creating ring shot enemy %f", ranged_stats.spread);
        new_obj = new RingShotEnemy(static_cast<Collider*>(obj), ranged_stats);
        SDL_Log("ring shot enemy spread %s", new_obj->GetID().c_str());
    }
    if (types->Attribute("dog") != nullptr) {
        new_obj = new Dog(static_cast<Collider*>(obj), ranged_stats);
    }
    if (types->Attribute("goblin") != nullptr) {
        new_obj = new Goblin(static_cast<Collider*>(obj), ranged_stats);
    }
    if (types->Attribute("helix_enemy") != nullptr) {
        new_obj = new HelixEnemy(static_cast<Collider*>(obj), ranged_stats);
    }
    if (types->Attribute("skeleton") != nullptr) {
        new_obj = new Skeleton(static_cast<Collider*>(obj), ranged_stats);
    }
    if (types->Attribute("mage") != nullptr) {
        new_obj = new Mage(static_cast<Collider*>(obj), ranged_stats);
    }

    delete obj;

    return new_obj;
}

GameObject* BuildEntrance(tinyxml2::XMLElement* types,
                          tinyxml2::XMLElement* xmlObj, GameObject* obj) {

    SDL_Log("entrance has next room id: %s", types->Attribute("next_room_id"));
    GameObject* new_obj = new Entrance(
        static_cast<Collider*>(obj), types->Attribute("curr_room_id"),
        types->Attribute("next_room_id"), atoi(types->Attribute("next_x")),
        atoi(types->Attribute("next_y")));

    return new_obj;
}

GameObject* BuildObjectOnType(tinyxml2::XMLElement* types,
                              tinyxml2::XMLElement* xmlObj, GameObject* obj) {
    GameObject* new_obj = obj;
    GameObject* to_delete = obj;

    if (types->Attribute("collider") != nullptr) {
        new_obj = LoadCollider(xmlObj, new_obj);
        SDL_Log("loaded collider: %d aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                static_cast<int>(dynamic_cast<Collider*>(new_obj) != nullptr));
    }

    if (types->Attribute("player") != nullptr) {
        SDL_Log("Adding new player");
        to_delete = new_obj;
        new_obj = new Player(static_cast<Collider*>(new_obj));

        delete to_delete;
        return new_obj;
    }

    if (types->Attribute("slime") != nullptr) {
        EnemyStats const stats =
            GetEnemyStats(xmlObj->FirstChildElement("EnemyStats"));
        to_delete = new_obj;
        new_obj = new Slime(static_cast<Collider*>(new_obj), stats);
        delete to_delete;
    }

    if (types->Attribute("charger") != nullptr) {
        EnemyStats const stats =
            GetEnemyStats(xmlObj->FirstChildElement("EnemyStats"));
        to_delete = new_obj;
        new_obj = new Charger(static_cast<Collider*>(new_obj), stats);
        delete to_delete;
    }

    if (types->Attribute("kamikaze") != nullptr) {
        EnemyStats const stats =
            GetEnemyStats(xmlObj->FirstChildElement("EnemyStats"));
        to_delete = new_obj;
        new_obj = new Kamikaze(static_cast<Collider*>(new_obj), stats);
        delete to_delete;
    }

    if (types->Attribute("ranged_enemy") != nullptr) {
        new_obj = BuildRangedEnemy(types, xmlObj, new_obj);
    }

    if (types->Attribute("entrance") != nullptr) {
        new_obj = BuildEntrance(types, xmlObj, new_obj);
    }

    if (types->Attribute("trap") != nullptr) {
        to_delete = new_obj;
        new_obj = new Trap(static_cast<Collider*>(new_obj),
                           std::stoi(types->Attribute("damage")));
        delete to_delete;
    }

    return new_obj;
}

std::vector<GameObject*> LoadObjects(const char* filepath) {
    tinyxml2::XMLDocument doc;
    std::vector<GameObject*> objects;

    SDL_Log("%s", filepath);

    tinyxml2::XMLError const error = doc.LoadFile(filepath);
    if (error != tinyxml2::XML_SUCCESS) {
        std::string what =
            "Could not load objects file: " + std::string(filepath);
        SDL_LogError(0, what.c_str());
        return objects;
    }

    tinyxml2::XMLElement* root = doc.FirstChildElement("Root");
    tinyxml2::XMLElement* curr_object = root->FirstChildElement("Object");
    tinyxml2::XMLElement* types = nullptr;

    if (root == nullptr) {
        SDL_Log("Root element does not exist for room %s", filepath);
        assert(false);
    }
    if (curr_object == nullptr) {
        SDL_Log("No objects exist in room %s", filepath);
        return objects;
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
            SDL_Log("failed to load object");
            assert(false);
            return objects;
        }
        SDL_Log("types collider attribute: %s", types->Attribute("Collider"));

        if (created_obj == nullptr) {
            SDL_Log("maybe collider load failed?");
            assert(false);
        };

        created_obj = BuildObjectOnType(types, curr_object, created_obj);
        // std::cout << created_obj->GetStateType() << std::endl;
        objects.push_back(created_obj);

        curr_object = curr_object->NextSiblingElement("Object");
    }

    return objects;
}

bool LoadTextures(const char* projectPath) {
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
            Renderer::GetInstance().AddTileMap(id, texture_path, tile_size,
                                               rows, cols);
        } else {
            Renderer::GetInstance().AddTexture(id, texture_path);
        }

        curr_texture = curr_texture->NextSiblingElement("Texture");
    }
    return true;
}

std::string LoadStartRoom(const char* path, int& x, int& y) {
    std::string room_order_path = path;
    room_order_path += "/start_room.txt";

    std::string line;

    std::ifstream file;

    std::string start_room;

    file.open(room_order_path.c_str());

    if (file.is_open()) {
        getline(file, line);
        start_room = line;

        getline(file, line);
        x = atoi(line.c_str());
        getline(file, line);
        y = atoi(line.c_str());
    } else {
        SDL_Log("FAILED TO LOAD ROOM ORDER FILE");
    }

    file.close();
    return start_room;
}

std::pair<int, int> LoadStartPosition(const char* path) {
    std::string line;

    std::ifstream file;

    SDL_Log("path: %s", path);
    file.open(path);

    int x = 0;
    int y = 0;

    if (file.is_open()) {
        getline(file, line);
        x = atoi(line.c_str());
        getline(file, line);
        y = atoi(line.c_str());
    } else {
        SDL_Log("FAILED TO LOAD START POSITION FILE");
    }
    SDL_Log("start position: %d %d", x, y);

    file.close();
    return {x, y};
}
