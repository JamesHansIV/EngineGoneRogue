#include "Config.h"
#include "Engine/Objects/Collider.h"

void WriteColliderInfo(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* xmlObj,
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

void WriteAnimationInfo(tinyxml2::XMLDocument& doc,
                        tinyxml2::XMLElement* xmlObj, GameObject* obj) {
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

void WriteBaseObjectInfo(tinyxml2::XMLDocument& doc,
                         tinyxml2::XMLElement* xmlObj, GameObject* obj) {
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

bool SaveObjects(const char* filepath, std::vector<GameObject*> objects) {
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
            type = doc.NewElement("Collider");
            type->SetText("True");
            types->InsertEndChild(type);
        }
        root->InsertEndChild(curr_xml_object);
    }

    return doc.SaveFile(filepath);
}
