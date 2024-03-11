#pragma once

#include<SDL2/SDL.h>
#include<map>
#include<vector>

// add default map 
// add save to map file
// add load from map file (one editor load and file modified);

enum class InputType { CONTINUOUS, LATCH };

enum class EditorAction { 
    ExitCurrentTool, 
    EnterDrawTool, 
    EnterEraseTool,
    PanCameraUp,
    PanCameraRight,
    PanCameraLeft, 
    PanCameraDown
};

struct Bind {
    EditorAction action;
    InputType input_type;
    std::vector<SDL_KeyCode> keys;
};

class KeyMap {
    public:
        KeyMap();
        ~KeyMap();

        std::map<EditorAction, Bind> m_Bindings_Map;
        std::map<EditorAction, Bind> m_Default_Bindings_Map;

        void BuildMapFromFile();
        bool CheckInputs(EditorAction action);

        // TODO
        bool WriteMapToFile();
        void SetBind(EditorAction action, std::vector<SDL_KeyCode> keys, InputType input_type);
        void DeleteBind(EditorAction action);

        static Bind CreateBind(EditorAction action, InputType input_type, std::vector<SDL_KeyCode> keys);

};