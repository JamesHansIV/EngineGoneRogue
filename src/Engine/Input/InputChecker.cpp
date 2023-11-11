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

bool InputChecker::isKeyPressed(int keycode) {
    return keys[keycode % kNumkeys];
}

void InputChecker::setKeyPressed(int keycode, bool value) {
    keys[keycode % kNumkeys] = value;
}

bool InputChecker::isMouseButtonPressed(int button) {
    return mouse_buttons[button % kNummousebuttons];
}

void InputChecker::setMouseButtonPressed(int button, bool value) {
    mouse_buttons[button % kNummousebuttons] = value;
}

int InputChecker::getMouseX() {
    return mouse_x;
}

int InputChecker::getMouseY() {
    return mouse_y;
}

void InputChecker::updateMousePosition(int x, int y) {
    mouse_x = x;
    mouse_y = y;
}