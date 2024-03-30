#pragma once

#include <vector>
#include <string>
#include "Engine/Editor/ActionRecordHandling/EditorAction.h"
#include "Engine/KeyMap/KeyMap.h"

class Toolbar {
    public:
    Toolbar(KeyMap* key_map);
    ~Toolbar();

    void InitActionDescriptionBindPairs();
    void InitActionToDescriptionMap();
    void InitKeyCodeToStringMap();
    void BuildHelpPopupText(KeyMap* key_map);

    std::vector<std::pair<std::string, EditorAction>>action_description_bind_pairs;
    std::unordered_map<EditorAction, std::string>action_to_description_map;
    std::string help_popup_text;

    private:
    std::unordered_map<SDL_KeyCode, std::string>keycode_to_string_map;
};
