#pragma once

#include<SDL2/SDL.h>
#include<vector>
#include<unordered_set>
#include<unordered_map>

#include "Engine/Editor/ActionRecordHandling/EditorAction.h"

// add default map 
// add save to map file
// add load from map file (one editor load and file modified);

enum class InputType { CONTINUOUS, LATCH, ON_UP_LATCH};

enum class MacKeys { LSHIFT, RSHIFT, CONTROL, LOPTION, ROPTION, LCOMMAND, RCOMMAND, DELETE };

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

        std::unordered_map<EditorAction, Bind> GetBindingMap() { return m_Bindings_Map; }

    private:
        std::unordered_map<EditorAction, Bind> m_Bindings_Map;
        // std::map<EditorAction, Bind> m_Default_Bindings_Map;

        static std::unordered_set<SDL_KeyCode>InitModifierKeys();
        void InitMacKeysToSDLMap();
};