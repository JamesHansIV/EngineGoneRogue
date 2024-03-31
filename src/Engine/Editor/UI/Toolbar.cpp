#include "Toolbar.h"

#include<iostream>

Toolbar::~Toolbar() {

}

Toolbar::Toolbar(KeyMap* key_map) {
    InitActionDescriptionBindPairs();
    InitActionToDescriptionMap();
    InitKeyCodeToStringMap();
    BuildHelpPopupText(key_map);
}

void Toolbar::InitKeyCodeToStringMap() {
    // letters
    for (int i = 0; i < 26; i++) 
        keycode_to_string_map.insert({static_cast<SDL_KeyCode>(97+i), std::string(1,char(97+i))});

    // nums
    for (int i = 0; i < 10; i++)
        keycode_to_string_map.insert({static_cast<SDL_KeyCode>(48+i), std::string(1,char(48+i))});
    
    // arrow keys
    keycode_to_string_map.insert({SDLK_DOWN, "DOWN"});
    keycode_to_string_map.insert({SDLK_UP, "UP"});
    keycode_to_string_map.insert({SDLK_RIGHT, "RIGHT"});
    keycode_to_string_map.insert({SDLK_LEFT, "LEFT"});

    // modifier keys
    keycode_to_string_map.insert({SDLK_LGUI, "command"});
    keycode_to_string_map.insert({SDLK_LSHIFT, "shift"});

    // misc
    keycode_to_string_map.insert({SDLK_ESCAPE, "esc"});
    keycode_to_string_map.insert({SDLK_BACKSPACE, "delete"});
}

void Toolbar::InitActionDescriptionBindPairs() {
    action_description_bind_pairs = {
        {"Exit current tool \n   & deselect all", EditorAction::EXIT_CURRENT_TOOL},
        {"Paint select tool", EditorAction::ENTER_TILE_SELECT_TOOL},
        {"Move selection",EditorAction::ENTER_SELECTION_MOVE_TOOL},
        {"Draw tool",EditorAction::ENTER_DRAW_TOOL},
        {"Paint button tool",EditorAction::ENTER_PAINT_BUCKET_TOOL},
        {"Erase tool", EditorAction::ENTER_ERASE_TOOL},
        {"Delete selection", EditorAction::EXECUTE_DELETE_SELECTION},
        {"Pan camera down", EditorAction::PAN_CAMERA_DOWN},
        {"Pan camera up", EditorAction::PAN_CAMERA_UP},
        {"Pan camera right", EditorAction::PAN_CAMERA_RIGHT},
        {"Pan camera left", EditorAction::PAN_CAMERA_LEFT},
        {"Copy selection", EditorAction::COPY_SELECTION},
        {"Cut Selection", EditorAction::CUT_SELECTION},
        {"Paste clipboard", EditorAction::PASTE_CLIPBOARD},
        {"Undo", EditorAction::UNDO_ACTION},
        {"Redo", EditorAction::REDO_ACTION},
        {"Save room", EditorAction::SAVE_ROOM}
    };
}

void Toolbar::InitActionToDescriptionMap() {
    for (auto [description, action] : action_description_bind_pairs)
        action_to_description_map.insert({action, description});
}

void Toolbar::BuildHelpPopupText(KeyMap* key_map) {
    for (auto pair : action_description_bind_pairs) {
        auto description = pair.first;
        auto editor_action = pair.second;
        auto keys = key_map->GetBindingMap()[editor_action].keys;

        std::string str = description + ": ";
        for (SDL_KeyCode key : keys) {
            if (key == keys.front())
                str += keycode_to_string_map.at(key);
            else 
                str += " + " + keycode_to_string_map.at(key);
            
            if (key == keys.back())
                str += "\n\n";
        }
        help_popup_text += str;
    }
}