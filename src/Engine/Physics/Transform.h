#pragma once
#include "Vector2D.h"

class Transform{
    public:
        float* X;
        float* Y;
        Transform(float* x, float* y) : X(x), Y(y){}
        void Log(const std::string& msg = "") const{
            std::cout << msg << "(X Y) = (" << *X << " " << *Y << ")"<< '\n';
        }
        inline float GetX() const { return *X; }
        inline float GetY() const { return *Y; }
        inline void TranslateX(float x) const{*X += x;}
        inline void TranslateY(float y) const{*Y += y;}
        inline void Translate(Vector2D v) const{*X += v.X; *Y += v.Y;}
};
