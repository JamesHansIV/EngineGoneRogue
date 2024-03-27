#include "ActionRecordHandler.h"
// #include "Engine/Editor/Editor.h"
#include "Engine/Objects/GameObject.h"
#include "Engine/Objects/IObject.h"
#include "Engine/Objects/Characters/Player.h"
#include <iostream>

// TODO
// switch stack to vector, so that we can impllmemnt a max size and delete from the back
// of the vector as new action records come in. max size could be elements or 
// mem size. The concern here is that some actions will be mem heavy (painting)
// and one paint could easily be the same size as 10-30 draws

ActionRecordHandler::~ActionRecordHandler() {
    while (!m_redo_stack.empty()) {
        delete m_redo_stack.top();
        m_redo_stack.pop();
    }
}

void ActionRecordHandler::RecordAction(ActionRecord* record) {
    m_undo_stack.push(record);
}

void ActionRecordHandler::UndoAction(std::vector<std::vector<GameObject*>>& layers) {
    if (m_undo_stack.empty())
        return;
    
    // get ptr & pop
    ActionRecord* record = m_undo_stack.top();
    m_undo_stack.pop();

    // push to redo stack
    m_redo_stack.push(record);

    // undo the action in the record
    switch(record->GetAction()) {
        case EditorAction::EXECUTE_DRAW:
            UndoDraw(record, layers[record->GetLayer()]);
            break;
        case EditorAction::EXECUTE_PAINT_BUCKET:
            UndoPaintBucket(record, layers[record->GetLayer()]);
            break;
        case EditorAction::EXECUTE_ERASE:
            UndoErase(record, layers[record->GetLayer()]);
            break;
        case EditorAction::EXECUTE_DRAG_MOVE:
            UndoDragMove(record, layers[record->GetLayer()]);
            break;
        default:
            std::cout << "not implemented yet undo action\n";
    }

    // std::cout << "*UNDO: " << m_undo_stack.size() << " | REDO: " << m_redo_stack.size() << std::endl;
}

void ActionRecordHandler::RedoAction(std::vector<std::vector<GameObject*>>& layers) {
    if (m_redo_stack.empty())
        return;
    
    ActionRecord* record = m_redo_stack.top();
    m_redo_stack.pop();

    m_undo_stack.push(record);
    
    // undo the action in the record
    switch(record->GetAction()) {
        case EditorAction::EXECUTE_DRAW:
            RedoDraw(record, layers[record->GetLayer()]);
            break;
        case EditorAction::EXECUTE_PAINT_BUCKET:
            RedoPaintBucket(record, layers[record->GetLayer()]);
            break;
        case EditorAction::EXECUTE_ERASE:
            RedoErase(record, layers[record->GetLayer()]);
            break;
        case EditorAction::EXECUTE_DRAG_MOVE:
            UndoDragMove(record, layers[record->GetLayer()]);
            break;
        default:
            std::cout << "not implemented yet redo action\n";
    }

    // std::cout << "UNDO: " << m_undo_stack.size() << " | *REDO: " << m_redo_stack.size() << std::endl;
}

void ActionRecordHandler::UndoDraw(ActionRecord* record, std::vector<GameObject*>&layer) {
    // create new pointer to obj
    GameObject* obj = record->GetObjects().front();
    GameObject* obj_copy = new GameObject(obj);
    
    // delete old ptr
    auto iter = std::find(layer.begin(), layer.end(), obj);
    if (iter != layer.end())
        layer.erase(iter);

    obj = nullptr;
    delete obj;

    record->SetObject(obj_copy,0);
}

void ActionRecordHandler::UndoErase(ActionRecord* record, std::vector<GameObject*>& layer) {
    RedoPaintBucket(record, layer);
}

void ActionRecordHandler::UndoPaintBucket(ActionRecord* record, std::vector<GameObject*>& layer) {
    // create a new pointer for each obj
    std::vector<GameObject*>object_copies;
    for (GameObject* obj : record->GetObjects()) {
        object_copies.emplace_back(new GameObject(obj));
    }

    // delete all the old ptrs
    int i = 0;
    for (GameObject* obj : record->GetObjects()) {
        auto iter = std::find(layer.begin(), layer.end(), obj);
        if (iter != layer.end())
            layer.erase(iter);
        
        obj = nullptr;
        delete obj;

        record->SetObject(object_copies[i],i);

        i++;
    }
}

void ActionRecordHandler::UndoDragMove(ActionRecord* record, std::vector<GameObject*>& layer) {
    TileCoords orig_coords = record->GetOrigCoords();
    TileCoords dst_coords = record->GetDstCoords();
    
    TileCoords delta_coords = dst_coords - orig_coords;

    // sanity check
    if (record->GetObjects().size() != record->GetObjectPositions().size())
        throw std::runtime_error("Object vector and Positions vector size mismatch!");

    // update positions
    std::vector<std::pair<float, float>>obj_origins = record->GetObjectPositions();
    for (int i=0; GameObject* obj : record->GetObjects()) {
        const float curr_x = obj->GetX();
        const float curr_y = obj->GetY();

        obj->SetX(obj_origins[i].first);
        obj->SetY(obj_origins[i].second);

        // update record for redo
        record->SetObjectPos({curr_x, curr_y}, i);

        i++;
    }

    return;

    // invert coords
    // record->SwapOrigAndDstCoords();
}


void ActionRecordHandler::RedoDraw(ActionRecord* record, std::vector<GameObject*>&layer) {
    GameObject* obj = record->GetObjects().front();
    GameObject* new_obj;

    if(obj == nullptr) 
        return;
    
    char id_suffix = 'a' + rand()%26;
    std::string id = obj->GetID() + "_" + id_suffix;
    // std::cout << "ID " << id << std::endl;

    Properties props(obj->GetTextureID(), obj->GetTilePos(), obj->GetDstRect(),
                    obj->GetRotation(), id);

    switch (obj->GetObjectType()) {
        case ObjectType::Base:
            new_obj = new GameObject(props);
            break;
        case ObjectType::Player:
            new_obj = new Player(props);
            break;
        case ObjectType::Enemy:
            new_obj = new GameObject(props);
            break;
        default:
            std::cout << "Invalid object type\n";
            assert(false);
            break;
    }

    record->SetObject(new_obj,0);

    layer.push_back(new_obj);
}

void ActionRecordHandler::RedoErase(ActionRecord* record, std::vector<GameObject*>& layer) {
    UndoPaintBucket(record, layer);
}

void ActionRecordHandler::RedoPaintBucket(ActionRecord* record, std::vector<GameObject*>& layer) {
    std::vector<GameObject*>new_objects;
    
    int i = 0;
    for (GameObject* obj : record->GetObjects()) {
        if (obj == nullptr)
            continue;

        GameObject* new_obj;
        
        char id_suffix = 'a' + rand()%26;
        std::string id = obj->GetID() + "_" + id_suffix;

        Properties props(obj->GetTextureID(), obj->GetTilePos(), obj->GetDstRect(),
                    obj->GetRotation(), id);

        switch (obj->GetObjectType()) {
            case ObjectType::Base:
                new_obj = new GameObject(props);
                break;
            case ObjectType::Player:
                new_obj = new Player(props);
                break;
            case ObjectType::Enemy:
                new_obj = new GameObject(props);
                break;
            default:
                std::cout << "Invalid object type\n";
                assert(false);
                break;
        }

        record->SetObject(new_obj,i);
        layer.push_back(new_obj);

        i++;
    }
}


void ActionRecordHandler::PrintStack(std::stack<ActionRecord*> stack) {
    std::cout << "TOP: " << stack.top() << "\t| SIZE: " << stack.size() << std::endl;
}