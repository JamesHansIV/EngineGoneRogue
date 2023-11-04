#pragma once

class InputChecker {
public:
    static bool isKeyPressed(int keycode);
    static void setKeyPressed(int keycode, bool value);
    static bool isMouseButtonPressed(int button);
    static void setMouseButtonPressed(int button, bool value);
    static int getMouseX();
    static int getMouseY();
    static void updateMousePosition(int x, int y);
};
