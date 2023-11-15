#pragma once

class InputChecker {
public:
    static bool IsKeyPressed(int keycode);
    static void SetKeyPressed(int keycode, bool value);
    static bool IsMouseButtonPressed(int button);
    static void SetMouseButtonPressed(int button, bool value);
    static int GetMouseX();
    static int GetMouseY();
    static void UpdateMousePosition(int x, int y);
    static int GetMouseWheelDirection();
    static void SetMouseWheelDirection(int direction);
};
