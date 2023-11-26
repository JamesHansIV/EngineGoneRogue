#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <iostream>
#include <string>

class Vector2D {
   public:
    float X;
    float Y;
    explicit Vector2D(float x = 0, float y = 0) : X(x), Y(y){};

    inline Vector2D operator+(const Vector2D& v2) const {
        return Vector2D(X + v2.X, Y + v2.Y);
    };

    inline Vector2D operator-(const Vector2D& v2) const {
        return Vector2D(X - v2.X, Y - v2.Y);
    };

    inline Vector2D operator*(const float scalar) const {
        return Vector2D(X * scalar, Y * scalar);
    }

    inline float GetMagnitude() const {
        return sqrt(X * X + Y * Y);
    }

    void Log(const std::string& msg = "") const {
        std::cout << msg << "(X Y) = (" << X << " " << Y << ")" << '\n';
    }

   private:
};
