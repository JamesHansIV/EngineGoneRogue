#include "Config.h"
#include "Engine/Objects/Collider.h"
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

int SaveObjects(const char* filepath, std::vector<GameObject*> objects) {
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* root = doc.NewElement("Root");
    doc.InsertFirstChild(root);

    tinyxml2::XMLElement* curr_xml_object;
    tinyxml2::XMLElement* types;
    tinyxml2::XMLElement* type;

    for (auto* obj : objects) {
        curr_xml_object = doc.NewElement("Object");
        types = doc.NewElement("Types");
        curr_xml_object->InsertEndChild(types);

        WriteBaseObjectInfo(doc, curr_xml_object, obj);

        if (obj->GetAnimation() != nullptr) {
            WriteAnimationInfo(doc, curr_xml_object, obj);
        }

        if (auto* collider = dynamic_cast<Collider*>(obj)) {
            WriteColliderInfo(doc, curr_xml_object, collider);
            types->SetAttribute("Collider", "1");
        }
        root->InsertEndChild(curr_xml_object);
    }

    return doc.SaveFile(filepath);
}

int SaveTextureMap(const char* filepath,
                   std::map<std::string, Texture*> textureMap) {
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

    for (auto& it : textureMap) {
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

    if (animation != nullptr) {
        obj->SetAnimation(new Animation());

        tinyxml2::XMLElement* curr =
            animation->FirstChildElement("AnimationInfo");

        while (curr != nullptr) {
            TilePos const tile_pos = {
                atoi(curr->Attribute("row")),
                atoi(curr->Attribute("col")),
                atoi(curr->Attribute("w")),
                atoi(curr->Attribute("h")),
            };

            AnimationInfo info = {curr->Attribute("texture_id"), tile_pos,
                                  atoi(curr->Attribute("frame_count")),
                                  atoi(curr->Attribute("speed"))};
            if (curr->Attribute("flip") != nullptr) {
                info.Flip = (SDL_RendererFlip)atoi(curr->Attribute("flip"));
            }

            info.Loop = curr->Attribute("loop") == "1" ? true : false;

            if (curr->Attribute("key_frames_start") != nullptr &&
                curr->Attribute("key_frames_end") != nullptr) {
                info.KeyFramesStart = atoi(curr->Attribute("key_frames_start"));
                info.KeyFramesEnd = atoi(curr->Attribute("key_frames_end"));
            }

            obj->GetAnimation()->AddAnimation(curr->Attribute("id"), info);

            SDL_Log("Animation row: %d", obj->GetAnimation()->GetSpriteRow());
            SDL_Log("Animation col: %d", obj->GetAnimation()->GetSpriteCol());
            SDL_Log("Animation frameCount: %d",
                    obj->GetAnimation()->GetFrameCount());
            SDL_Log("Animation speed: %d",
                    obj->GetAnimation()->GetAnimationSpeed());
        }
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
    SDL_Log("in load collider");
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
    }

    collider->GetCollisionBox().Set(atoi(collision_box->Attribute("x")),
                                    atoi(collision_box->Attribute("y")),
                                    atoi(collision_box->Attribute("w")),
                                    atoi(collision_box->Attribute("h")));

    delete obj;
    return collider;
}

std::vector<GameObject*> LoadObjects(const char* filepath) {
    tinyxml2::XMLDocument doc;
    std::vector<GameObject*> objects;

    SDL_Log("%s", filepath);

    tinyxml2::XMLError const error = doc.LoadFile(filepath);
    if (error != tinyxml2::XML_SUCCESS) {
        SDL_LogError(0, "Could not load objects");
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

        if (types->Attribute("Collider") != nullptr) {
            created_obj = LoadCollider(curr_object, created_obj);
            SDL_Log("collider");
            SDL_Log("loaded collider: %d aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
                    static_cast<int>(dynamic_cast<Collider*>(created_obj) !=
                                     nullptr));
        }

        if (created_obj == nullptr) {
            SDL_Log("maybe collider load failed?");
            assert(false);
        };

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
            Renderer::GetInstance()->AddTileMap(id, texture_path, tile_size,
                                                rows, cols);
        } else {
            Renderer::GetInstance()->AddTexture(id, texture_path);
        }

        curr_texture = curr_texture->NextSiblingElement("Texture");
    }
    return true;
}
