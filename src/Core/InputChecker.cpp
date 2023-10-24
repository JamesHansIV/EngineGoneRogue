#include "InputChecker.h"

const int NUMKEYS = 322;
bool keys[NUMKEYS];

bool InputChecker::isKeyPressed(int keycode) {
    return keys[keycode % NUMKEYS];
}

void InputChecker::setKeyPressed(int keycode, bool value) {
    keys[keycode % NUMKEYS] = value;
}