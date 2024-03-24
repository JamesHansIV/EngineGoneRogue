#include "KeyMap.h"
#include "Engine/Input/InputChecker.h"

#include<fstream>
#include<iostream>
#include<string>
#include <utility>
#include<vector>
#include<unordered_map>
#include<unordered_set>

std::string path = "editor.binds";

std::ostream& operator << (std::ostream& os, const EditorAction& obj)
{
   os << static_cast<std::underlying_type_t<EditorAction>>(obj);
   return os;
}

std::ostream& operator << (std::ostream& os, const InputType& obj)
{
   os << static_cast<std::underlying_type_t<InputType>>(obj);
   return os;
}

Bind KeyMap::CreateBind(EditorAction action, InputType input_type, std::vector<SDL_KeyCode> keys) {
    int modifier_key_count = 0;

    for (SDL_KeyCode key : keys) {
        if (modifier_keys.contains(key))
            modifier_key_count++;
    }

    Bind bind = {action, input_type, std::move(keys), modifier_key_count};

    return bind;
}


KeyMap::KeyMap() {
    std::cout << "INIT KEYBINDS\n";
    InitModifierKeys();

    BuildMapFromFile();

    // std::cout << "mapped key " << mac_keys_to_sdl[MacKeys::LCOMMAND] << "\n";


    // std::cout << "\n";
    // for (const auto& [ action, bind ] : default_bindings) {
    //     std::cout << "\nACTION " << action << ":";
    //     if (static_cast<int>(action) != 10) std::cout << " ";
    //     std::cout << "  n_mod_keys: " << bind.num_modifier_keys;
    //     for (auto key : bind.keys) {
    //         std::cout << "    " << key;
    //     }
    // }
    // std::cout << "\n";
}

KeyMap::~KeyMap() = default;

std::unordered_map<MacKeys, SDL_KeyCode> KeyMap::mac_keys_to_sdl = {
    {MacKeys::LSHIFT, SDLK_LSHIFT}, 
    {MacKeys::RSHIFT, SDLK_RSHIFT}, 
    {MacKeys::CONTROL, SDLK_LCTRL}, 
    {MacKeys::LOPTION, SDLK_LALT},  
    {MacKeys::ROPTION, SDLK_RALT},  
    {MacKeys::LCOMMAND, SDLK_LGUI}, 
    {MacKeys::RCOMMAND, SDLK_RGUI}  
};

std::unordered_set<SDL_KeyCode>KeyMap::modifier_keys = InitModifierKeys();

std::unordered_map<EditorAction, Bind> KeyMap::default_bindings = {
    // Tool selection
    {EditorAction::EXIT_CURRENT_TOOL, CreateBind(EditorAction::EXIT_CURRENT_TOOL, InputType::LATCH, std::vector<SDL_KeyCode>{SDLK_ESCAPE})},
    {EditorAction::ENTER_DRAW_TOOL, CreateBind(EditorAction::ENTER_DRAW_TOOL, InputType::LATCH, std::vector<SDL_KeyCode>{SDLK_d})}, 
    {EditorAction::ENTER_ERASE_TOOL, CreateBind(EditorAction::ENTER_ERASE_TOOL, InputType::LATCH, std::vector<SDL_KeyCode>{SDLK_e})},
    {EditorAction::ENTER_TILE_SELECT_TOOL, CreateBind(EditorAction::ENTER_TILE_SELECT_TOOL, InputType::LATCH, std::vector<SDL_KeyCode>{SDLK_t})},
    {EditorAction::ENTER_SELECTION_MOVE_TOOL, CreateBind(EditorAction::ENTER_SELECTION_MOVE_TOOL, InputType::LATCH, std::vector<SDL_KeyCode>{SDLK_m})},
    
    // Camera controls
    {EditorAction::PAN_CAMERA_DOWN, CreateBind(EditorAction::PAN_CAMERA_DOWN, InputType::CONTINUOUS, std::vector<SDL_KeyCode>{SDLK_DOWN})},
    {EditorAction::PAN_CAMERA_UP, CreateBind(EditorAction::PAN_CAMERA_UP, InputType::CONTINUOUS, std::vector<SDL_KeyCode>{SDLK_UP})},
    {EditorAction::PAN_CAMERA_RIGHT, CreateBind(EditorAction::PAN_CAMERA_RIGHT, InputType::CONTINUOUS, std::vector<SDL_KeyCode>{SDLK_RIGHT})},
    {EditorAction::PAN_CAMERA_LEFT, CreateBind(EditorAction::PAN_CAMERA_LEFT, InputType::CONTINUOUS, std::vector<SDL_KeyCode>{SDLK_LEFT})},

    // Copy and paste
    {EditorAction::COPY_SELECTION, CreateBind(EditorAction::COPY_SELECTION, InputType::LATCH, std::vector<SDL_KeyCode>{mac_keys_to_sdl[MacKeys::LCOMMAND], SDLK_c})},
    {EditorAction::PASTE_SELECTION, CreateBind(EditorAction::PASTE_SELECTION, InputType::LATCH, std::vector<SDL_KeyCode>{mac_keys_to_sdl[MacKeys::LCOMMAND], SDLK_v})},

    // Undo and Redo
    {EditorAction::UNDO_ACTION, CreateBind(EditorAction::UNDO_ACTION, InputType::ON_UP_LATCH, std::vector<SDL_KeyCode>{mac_keys_to_sdl[MacKeys::LCOMMAND], SDLK_z})},
    {EditorAction::REDO_ACTION, CreateBind(EditorAction::REDO_ACTION, InputType::ON_UP_LATCH, 
                                            std::vector<SDL_KeyCode>{mac_keys_to_sdl[MacKeys::LCOMMAND], mac_keys_to_sdl[MacKeys::LSHIFT], SDLK_z})}
};

std::unordered_set<SDL_KeyCode> KeyMap::InitModifierKeys() {
    auto keys = InputChecker::GetModifierKeyCodes();
    std::unordered_set<SDL_KeyCode>temp = {};
    for (int key : keys) {
        temp.insert(static_cast<SDL_KeyCode>(key));
    }

    return temp;
}

bool KeyMap::CheckInputs(EditorAction action) {

    Bind const bind = m_Bindings_Map[action];

    for (SDL_KeyCode const key : bind.keys) {
        bool const curr_down = InputChecker::IsKeyPressed(key);
        bool const prev_down = InputChecker::WasKeyAlreadyPresssed(key);

        // check modifiers, modifiers may already be pressed
        if (bind.num_modifier_keys != InputChecker::GetNumModifierKeysDown()) {
            return false;
        }

        // if one key is a modifier key ignore the latch check (for that key)
        if (modifier_keys.contains(key)) {
            if (!curr_down) {
                return false;
            }
                // return false;
            continue;
        }

        // check release type inputs
        if (bind.input_type == InputType::ON_UP_LATCH) {
            if (curr_down || !prev_down) {
                return false;
            }
        }

        // check latch type inputs
        if (bind.input_type == InputType::LATCH){
            if (prev_down || !curr_down) {
                return false;
            }
        }

        // check Continuous
        if (bind.input_type == InputType::CONTINUOUS) {
            if (!curr_down) return false;
        }
    }

    if (bind.input_type != InputType::CONTINUOUS)
        std::cout << "ACTION " << action << " SUCCCESS\n";
    return true;
}

void KeyMap::BuildMapFromFile() {
    std::fstream file;
    try {
        file.open(path);
        
        if (file.is_open()) {
            std::cout << "file open" << '\n';
        } else {
            throw std::runtime_error("failure to open keybinds (*.binds) file: " + path);
        }

        // read lines and add them to line vector
        std::string str;
        std::vector<std::string> lines;
        while(std::getline(file, str)) lines.push_back(str);

        // pull data out of each line
        for (std::string line : lines) {
            // remove whitespace
            line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char x) { return std::isspace(x); }), line.end());
            
            // seperate fields by delimiter ','
            std::vector<std::string>fields;
            int pos = 0;
            while(pos < line.size()){
                pos = line.find(',');
                fields.push_back(line.substr(0,pos));
                line.erase(0, pos+1); 
            }

            // creat a bind from the fields & add it to the map
            auto const bind_action = static_cast<EditorAction>(stoi(fields[0]));
            auto const bind_input_type = static_cast<InputType>(stoi(fields[1]));

            std::vector<SDL_KeyCode>bind_keys;
            for (int i = 2; i < fields.size(); i++) {
                bind_keys.push_back(static_cast<SDL_KeyCode>(stoi(fields[i])));
            }

            Bind const bind{bind_action, bind_input_type, bind_keys};

            m_Bindings_Map.insert(std::pair<EditorAction, Bind>(bind_action, bind));

        }

    }
    // if fail load default binds into map, then write defaults to file
    catch (const std::runtime_error& e) {
        std::cout << "ERROR: " << e.what() << '\n';
        std::cout << "Using defaults\n";
        m_Bindings_Map = default_bindings;
    }
}

