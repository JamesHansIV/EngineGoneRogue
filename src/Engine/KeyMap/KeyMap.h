#pragma once

#include<SDL2/SDL.h>
#include<vector>
#include<unordered_set>
#include<unordered_map>

// add default map 
// add save to map file
// add load from map file (one editor load and file modified);

enum class InputType { CONTINUOUS, LATCH, ON_UP_LATCH};

enum class MacKeys { LSHIFT, RSHIFT, CONTROL, LOPTION, ROPTION, LCOMMAND, RCOMMAND };

enum class EditorAction { 
    EXIT_CURRENT_TOOL, 
    ENTER_DRAW_TOOL, 
    ENTER_ERASE_TOOL,
    ENTER_TILE_SELECT_TOOL,
    PAN_CAMERA_UP,
    PAN_CAMERA_RIGHT,
    PAN_CAMERA_LEFT, 
    PAN_CAMERA_DOWN,
    COPY_SELECTION,
    PASTE_SELECTION,
    UNDO_ACTION,
    REDO_ACTION,
    ENTER_SELECTION_MOVE_TOOL
};

struct Bind {
    EditorAction action;
    InputType input_type;
    std::vector<SDL_KeyCode> keys;
    int num_modifier_keys;
};

class KeyMap {
    public:
        KeyMap();
        ~KeyMap();

        static std::unordered_set<SDL_KeyCode>modifier_keys;
        static std::unordered_map<MacKeys, SDL_KeyCode>mac_keys_to_sdl;
        static std::unordered_map<EditorAction, Bind>default_bindings;

        void BuildMapFromFile();
        bool CheckInputs(EditorAction action);

        // TODO
        bool WriteMapToFile();
        void SetBind(EditorAction action, std::vector<SDL_KeyCode> keys, InputType input_type);
        void DeleteBind(EditorAction action);

        static Bind CreateBind(EditorAction action, InputType input_type, std::vector<SDL_KeyCode> keys);

    private:
        std::unordered_map<EditorAction, Bind> m_Bindings_Map;
        // std::map<EditorAction, Bind> m_Default_Bindings_Map;

        static std::unordered_set<SDL_KeyCode>InitModifierKeys();
        void InitMacKeysToSDLMap();
};