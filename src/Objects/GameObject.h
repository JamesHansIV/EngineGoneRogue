#pragma once

#include "IObject.h"
#include "../Components/Transform.h"
#include <string>

struct Properties{
    public:
        Properties(std::string textureID, int x, int y, int width, int height, int tilemapIndex = 0, SDL_RendererFlip flip = SDL_FLIP_NONE){
            X = x;
            Y = y;
            Flip = flip;
            Width = width;
            Height = height;
            TilemapIndex = tilemapIndex;
            TextureID = textureID;
        }

        std::string TextureID;
        int Width, Height;
        int TilemapIndex;
        float X, Y;
        SDL_RendererFlip Flip;
};

class GameObject : public IObject {
    public:
        GameObject(Properties* props): m_TextureID(props->TextureID),
            m_Width(props->Width), m_Height(props->Height), m_Flip(props->Flip), m_ObjectID(0){

            m_Transform = new Transform(props->X, props->Y);
        }

        virtual void Draw()=0;
        virtual void Clean()=0;
        virtual void Update(float dt)=0;

        int GetID() { return m_ObjectID; }
        void SetID(int id) { m_ObjectID = id; }

    protected:
        Transform* m_Transform;
        int m_Width, m_Height;
        int TilemapIndex;
        std::string m_TextureID;
        int m_ObjectID;
        SDL_RendererFlip m_Flip;
};
