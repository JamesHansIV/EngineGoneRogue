#include "InputChecker.h"

const int NUMKEYS = 322;
const int NUMMOUSEBUTTONS = 3;
bool keys[NUMKEYS];
bool mouseButtons[NUMMOUSEBUTTONS];

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
