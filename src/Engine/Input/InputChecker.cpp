#include "InputChecker.h"
#include <SDL2/SDL.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <array>

const int kNumkeys = 322;
const int kNummousebuttons = 3;
// bool keys[kNumkeys];
std::array<bool, kNumkeys> keys;
std::array<bool, kNumkeys> prev_frame_keys;
// bool prev_frame_keys[kNumkeys];
bool mouse_buttons[kNummousebuttons];
int mouse_x;
int mouse_y;
int mouse_wheel_direction = 0;

int num_keys_down = 0;
int num_mod_keys_down = 0;

const std::unordered_set<int> InputChecker::modifier_keys = { 1073742055, 1073742051, 1073742054, 1073742050, 1073742052, 1073742048, 1073742053, 1073742049 };

bool InputChecker::IsKeyPressed(int keycode) {
    return keys[(keycode % kNumkeys)];
    // return keys.at(keycode % kNumkeys);
}

void InputChecker::SetKeyPressed(int keycode, bool value) {
    int const key = keycode % kNumkeys;
    keys[key] = value;

    if (modifier_keys.contains(keycode))
        num_mod_keys_down = (value == true) ? num_mod_keys_down + 1 : num_mod_keys_down - 1; // update keycount
}

bool InputChecker::IsMouseButtonPressed(int button) {
    return mouse_buttons[button % kNummousebuttons];
}

void InputChecker::SetMouseButtonPressed(int button, bool value) {
    mouse_buttons[button % kNummousebuttons] = value;
}

int InputChecker::GetMouseX() {
    return mouse_x;
}

int InputChecker::GetMouseY() {
    return mouse_y;
}

void InputChecker::UpdateMousePosition(int x, int y) {
    mouse_x = x;
    mouse_y = y;
}

void InputChecker::SetMouseWheelDirection(int direction) {
    mouse_wheel_direction = direction;
}

int InputChecker::GetMouseWheelDirection() {
    return mouse_wheel_direction;
}

void InputChecker::SetPrevFrameKeys() {
    prev_frame_keys = keys;
}

void InputChecker::ClearPrevFrameKeys() {
    std::fill(std::begin(prev_frame_keys), std::end(prev_frame_keys),0);
}

bool InputChecker::WasKeyAlreadyPresssed(int keycode) {
    return prev_frame_keys[(keycode % kNumkeys)];
}

int InputChecker::GetNumModifierKeysDown() {
    return num_mod_keys_down;
}

std::unordered_set<int> InputChecker::GetModifierKeyCodes() {
    return modifier_keys;
}