#include "ActionRecord.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Editor/TileCoords.h"

#include <unordered_map>

ActionRecord::ActionRecord(EditorAction action, GameObject* object, int layer) {
    m_action = action;
    m_layer = layer;
    m_objects.emplace_back(object);
}

ActionRecord::ActionRecord(EditorAction action, std::vector<GameObject*> objects, int layer) {
    m_action = action;
    m_layer = layer;
    for (auto& obj : objects)
        m_objects.emplace_back(obj);
}

ActionRecord::ActionRecord(EditorAction action, std::set<GameObject*> objects, int layer, TileCoords orig, TileCoords dst) {
    m_action = action;
    m_layer = layer;
    m_orig_coords = orig;
    m_dst_coords = dst;
    for (auto& obj: objects)
        m_objects.emplace_back(obj);
}

ActionRecord::ActionRecord(EditorAction action, std::unordered_map<GameObject*, std::pair<float, float>>object_origins_map, int layer, TileCoords orig, TileCoords dst) {
    m_action = action;
    m_layer = layer;
    m_orig_coords = orig;
    m_dst_coords = dst;
    for (auto& [obj, pos] : object_origins_map) {
        m_objects.emplace_back(obj);
        m_object_pos.emplace_back(pos);
    }
}

ActionRecord::~ActionRecord() {
    for (auto& obj : m_objects)
        delete obj;
    m_objects.clear();
}

void ActionRecord::SetObject(GameObject* object, int index) {
    if(index >= m_objects.size()) {
        m_objects.emplace_back(object);
    } else {
        m_objects.at(index) = object;
    }
}

void ActionRecord::SetObjectPos(std::pair<float, float>pos, int index) {
    if (index >= m_object_pos.size()) 
        throw std::runtime_error("Index out of bounds in ActionRecord::SetObjectPos call");
    
    m_object_pos.at(index) = pos;
}

void ActionRecord::SwapOrigAndDstCoords() {
    TileCoords temp = m_orig_coords;
    m_orig_coords = m_dst_coords;
    m_dst_coords = temp;
}