#include "InputChecker.h"
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>

const int kNumkeys = 322;
const int kNummousebuttons = 3;
bool keys[kNumkeys];
bool mouse_buttons[kNummousebuttons];
int mouse_x;
int mouse_y;
int mouse_wheel_direction = 0;

bool InputChecker::IsKeyPressed(int keycode) {
    return keys[keycode % kNumkeys];
}

void InputChecker::SetKeyPressed(int keycode, bool value) {
    keys[keycode % kNumkeys] = value;
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