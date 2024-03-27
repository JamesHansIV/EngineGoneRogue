#pragma once

#include "EditorAction.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Editor/TileCoords.h"

#include <vector>
#include <set>
// #include <pair>

class ActionRecord {
    public:
    ActionRecord(EditorAction action, GameObject* object, int layer);
    ActionRecord(EditorAction action, std::vector<GameObject*> objects, int layer);
    ActionRecord(EditorAction action, std::set<GameObject*> objects, int layer, TileCoords orig, TileCoords dst);
    ActionRecord(EditorAction action, std::unordered_map<GameObject*, std::pair<float, float>>object_origins_map, int layer, TileCoords orig, TileCoords dst);
    ~ActionRecord();

    EditorAction GetAction() { return m_action; }
    std::vector<GameObject*> GetObjects() { return m_objects; }
    int GetLayer() { return m_layer; }
    TileCoords GetOrigCoords() { return m_orig_coords; }
    TileCoords GetDstCoords() { return m_dst_coords; }
    std::vector<std::pair<float, float>> GetObjectPositions() { return m_object_pos; }

    void SetObject(GameObject* object, int index);
    void SetObjectPos(std::pair<float, float>pos, int index);
    void SwapOrigAndDstCoords();

    private:
    EditorAction m_action{};
    std::vector<GameObject*> m_objects{};
    int m_layer{0};
    TileCoords m_orig_coords;
    TileCoords m_dst_coords;
    std::vector<std::pair<float, float>> m_object_pos{};
};

/* 
draw // DONE
    * Add an object at x,y (px)
    * object is current obj
    * store: 
    *   - obj_id (need to add check in AddObject to ensure no id duplicates)
    *   - current_texture(id) // add safety net
    *   - info.tile
    *   - info.dstrect
    *   - info.rotattion
    *   - info.type

delete // DONE
    * obj* is deleted, so we must preserve its data in a copy
    * store:
    *   - All obj props

move    // DONE
paint   // DONE
colliders?
undo    // DONE
redo    // DONE
*/
