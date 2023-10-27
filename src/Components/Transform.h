#pragma once

#include "Component.h"
#include <SDL2/SDL.h>

struct Position : public Component {
    public:
        Position(int objectID, int row, int col, int dstX, int dstY) :
            Component(objectID), Row(row), Col(col), DstX(dstX), DstY(dstY) {}
        
        virtual ComponentType GetComponentType() const override { return ComponentType::Position; }
        int Row, Col;
        int DstX, DstY;
};

struct Dimensions : public Component {
    public:
        Dimensions(int objectID, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
            : Component(objectID), SrcWidth(srcWidth), SrcHeight(srcHeight),
            DstWidth(dstWidth), DstHeight(dstHeight){}

        virtual ComponentType GetComponentType() const override { return ComponentType::Dimensions; }
        int SrcWidth, SrcHeight;
        int DstWidth, DstHeight;
        
};

struct Rotate : public Component {
    public:
        Rotate(int objectID, double angle, SDL_RendererFlip flipped) : Component(objectID), Angle(angle), Flipped(flipped) {}
    
    virtual ComponentType GetComponentType() const override { return ComponentType::Rotate; }
    double Angle;
    SDL_RendererFlip Flipped;
};