#pragma once

#include "EditorAction.h"
#include "Engine/Objects/GameObject.h"

#include <vector>

class ActionRecord {
    public:
    ActionRecord(EditorAction action, GameObject* object, int layer);
    ActionRecord(EditorAction action, std::vector<GameObject*> objects, int layer);
    ~ActionRecord();

    EditorAction GetAction() { return m_action; }
    std::vector<GameObject*> GetObjects() { return m_objects; }
    int GetLayer() { return m_layer; }

    void SetObject(GameObject* object, int pos);

    private:
    EditorAction m_action{};
    std::vector<GameObject*> m_objects{};
    int m_layer{0};
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

move
paint   // DONE
colliders?
undo    // DONE
redo    // DONE
*/
