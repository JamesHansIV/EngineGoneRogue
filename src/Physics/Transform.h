#pragma once
#include "Vector2D.h"

class Transform{
    public:
        float X;
        float Y;
        Transform(float x=0, float y=0):X(x), Y(y){}
        void Log(std::string msg = ""){
            std::cout << msg << "(X Y) = (" << X << " " << Y << ")"<< std::endl;
        }
        inline void TranslateX(float x){X += x;}
        inline void TranslateY(float y){Y += y;}
        inline void Translate(Vector2D v){X += v.X; Y += v.Y;}
};
