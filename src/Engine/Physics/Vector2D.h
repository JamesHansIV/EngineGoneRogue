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

    inline float operator*(const Vector2D& v2) const {
        return X * v2.X + Y * v2.Y;
    }

    inline float GetMagnitude() const {
        return sqrt(X * X + Y * Y);
    }

    inline bool IsZero() const {
        return X == 0.0F && Y == 0.0F;
    }

    void Log(const std::string& msg = "") const {
        std::cout << msg << "(X Y) = (" << X << " " << Y << ")" << '\n';
    }

   private:
};

class Matrix2D {
    public:
        float x1;
        float x2;
        float x3;
        float x4;
        explicit Matrix2D(float a = 0, float b = 0, float c = 0, float d = 0)
            : x1(a), x2(b), x3(c), x4(d) {}

        inline Vector2D operator*(const Vector2D& v) const {
            return Vector2D(x1 * v.X + x2 * v.Y, x3 * v.X + x4 * v.Y);
        };

        inline Matrix2D operator*(float s) const {
            return Matrix2D(x1 * s, x2 * s, x3 * s, x4 * s);
        }

        inline float Determinant() const {
            return x1 * x4 - x2 * x3;
        }

        inline Matrix2D Inverse() {
            if (Determinant() == 0.0F) {
                SDL_Log("ERROR: determinant is 0 for matrix:");
                Log();
                return Matrix2D(0, 0, 0, 0);
            }
            return Matrix2D(x4, -x2, -x3, x1) * (1 / Determinant());
        }

        void Log(const std::string& msg = "") const {
            std::cout << msg << "(x1 x2) = (" << x1 << " " << x2 << ")" << '\n';
            std::cout << msg << "(x3 x4) = (" << x3 << " " << x4 << ")" << '\n';
        }
};