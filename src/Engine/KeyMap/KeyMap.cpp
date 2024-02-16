#include "KeyMap.h"
#include "Engine/Input/InputChecker.h"

#include<fstream>
#include<iostream>
#include<string>
#include <utility>
#include<vector>

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
    Bind bind = {action, input_type, std::move(keys)};
    return bind;
}


KeyMap::KeyMap() {
    m_Default_Bindings_Map = { 
        {EditorAction::EXIT_CURRENT_TOOL, CreateBind(EditorAction::EXIT_CURRENT_TOOL, InputType::LATCH, std::vector<SDL_KeyCode>{SDLK_ESCAPE})},
        {EditorAction::ENTER_DRAW_TOOL, CreateBind(EditorAction::ENTER_DRAW_TOOL, InputType::LATCH, std::vector<SDL_KeyCode>{SDLK_d})}, 
        {EditorAction::ENTER_ERASE_TOOL, CreateBind(EditorAction::ENTER_ERASE_TOOL, InputType::LATCH, std::vector<SDL_KeyCode>{SDLK_e})},
        {EditorAction::PAN_CAMERA_DOWN, CreateBind(EditorAction::PAN_CAMERA_DOWN, InputType::CONTINUOUS, std::vector<SDL_KeyCode>{SDLK_DOWN})},
        {EditorAction::PAN_CAMERA_UP, CreateBind(EditorAction::PAN_CAMERA_UP, InputType::CONTINUOUS, std::vector<SDL_KeyCode>{SDLK_UP})},
        {EditorAction::PAN_CAMERA_RIGHT, CreateBind(EditorAction::PAN_CAMERA_RIGHT, InputType::CONTINUOUS, std::vector<SDL_KeyCode>{SDLK_RIGHT})},
        {EditorAction::PAN_CAMERA_LEFT, CreateBind(EditorAction::PAN_CAMERA_LEFT, InputType::CONTINUOUS, std::vector<SDL_KeyCode>{SDLK_LEFT})}
    };

    BuildMapFromFile();

    Bind bind = m_Bindings_Map[EditorAction::ENTER_DRAW_TOOL];
    std::cout << " " << bind.keys[0] << " " << bind.action << '\n';
    std::cout << m_Bindings_Map.size() << '\n';
    std::cout << m_Default_Bindings_Map.size() << '\n';
}

KeyMap::~KeyMap() = default;

bool KeyMap::CheckInputs(EditorAction action) {
    bool const res = false;
    Bind const bind = m_Bindings_Map[action];

    for (SDL_KeyCode const key : bind.keys) {
        bool const curr_down = InputChecker::IsKeyPressed(key);
        bool const prev_down = InputChecker::WasKeyAlreadyPresssed(key);

        if (bind.input_type == InputType::LATCH && prev_down || !curr_down) {
            return false;
        }
    }
    // std::cout << "returning true" << std::endl;
    return true;
}

void KeyMap::BuildMapFromFile() {
    std::fstream file;
    try {
        file.open(path);
        
        if (file.is_open()) {
            std::cout << "file open" << '\n';
        } else {
            throw std::runtime_error("failure to open file!");
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
        m_Bindings_Map = m_Default_Bindings_Map;
    }
}

