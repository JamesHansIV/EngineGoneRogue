#pragma once

#include "ActionRecord.h"
#include "Engine/Objects/GameObject.h"

#include <stack>

class ActionRecordHandler {
    public:
    ActionRecordHandler (int tileSize) : m_tile_size(tileSize) {};
    ~ActionRecordHandler();

    void RecordAction(ActionRecord* record);
    void UndoAction(std::vector<std::vector<GameObject*>>& layers);
    void RedoAction(std::vector<std::vector<GameObject*>>& layers);

    private:
    std::stack<ActionRecord*> m_undo_stack;
    std::stack<ActionRecord*> m_redo_stack;

    int m_tile_size;

    // std::vector<std::vector<GameObject*>>* m_layers_ptr;

    void UndoDraw(ActionRecord* record, std::vector<GameObject*>& layer);
    void UndoErase(ActionRecord* record, std::vector<GameObject*>& layer);
    void UndoPaintBucket(ActionRecord* record, std::vector<GameObject*>& layer);
    void UndoDragMove(ActionRecord* record, std::vector<GameObject*>& layer);
    void UndoDeleteSelection(ActionRecord* record, std::vector<GameObject*>& layer);
    void UndoPasteClipboard(ActionRecord* record, std::vector<GameObject*>& layer);

    void RedoDraw(ActionRecord* record, std::vector<GameObject*>& layer);
    void RedoErase(ActionRecord* record, std::vector<GameObject*>& layer);
    void RedoPaintBucket(ActionRecord* record, std::vector<GameObject*>& layer);
    // void RedoDragMove(ActionRecord* record, std::vector<GameObject*>& layer);
    void RedoDeleteSelection(ActionRecord* record, std::vector<GameObject*>& layer);
    void RedoPasteClipboard(ActionRecord* record, std::vector<GameObject*>& layer);

    void PrintStack(std::stack<ActionRecord*> stack);
};