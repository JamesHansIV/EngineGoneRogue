#include "InputChecker.h"
#include <cmath>
#include <iostream>
#include <SDL2/SDL.h>

const int NUMKEYS = 322;
const int NUMMOUSEBUTTONS = 3;
bool keys[NUMKEYS];
bool mouseButtons[NUMMOUSEBUTTONS];
int mouseX;
int mouseY;

bool InputChecker::isKeyPressed(int keycode) {
    return keys[keycode % NUMKEYS];
}

void InputChecker::setKeyPressed(int keycode, bool value) {
    keys[keycode % NUMKEYS] = value;
}

bool InputChecker::isMouseButtonPressed(int button) {
    return mouseButtons[button % NUMMOUSEBUTTONS];
}

void InputChecker::setMouseButtonPressed(int button, bool value) {
    mouseButtons[button % NUMMOUSEBUTTONS] = value;
}

int InputChecker::getMouseX() {
    return mouseX;
}

int InputChecker::getMouseY() {
    return mouseY;
}

void InputChecker::updateMousePosition(int x, int y) {
    mouseX = x;
    mouseY = y;
}